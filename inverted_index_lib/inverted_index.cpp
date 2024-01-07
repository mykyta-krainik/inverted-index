#include "inverted_index.h"
#include "json.hpp"
#include <fstream>

using nlohmann::json;
using std::ofstream;
using std::wstring_convert;
using std::codecvt_utf8;

inverted_index::inverted_index() = default;

inverted_index::~inverted_index() {
    clear();
};

void inverted_index::add(const word& word, const document& document) {
    add_documents_to_word(word, {document});
}

void inverted_index::add(const word& word, const documents& docs) {
    add_documents_to_word(word, docs);
}

void inverted_index::add(const inv_index& idx) {
    for (const auto& pair : idx) {
        add_documents_to_word(pair.first, pair.second);
    }
}

const documents& inverted_index::find(const word& word) const {
    read_lock index_read_lock(index_mutex_);
    auto it = index_.find(word);

    if (it != index_.end()) {
        read_lock word_lock(index_word_mutexes_.at(word));
        return it->second;
    }

    static const documents empty_documents;
    return empty_documents;
}

bool inverted_index::contains(const word& word) const {
    read_lock index_read_lock(index_mutex_);
    return index_.contains(word);
}

void inverted_index::remove_word(const word& word) {
    write_lock index_write_lock(index_mutex_);

    if (!index_.contains(word)) {
        return;
    }

    write_lock word_lock(index_word_mutexes_.at(word));

    index_.erase(word);

    write_lock word_mutexes_lock(index_word_mutexes_mutex_);

    index_word_mutexes_.erase(word);
}

void inverted_index::remove_document_from_all_records(const document& doc) {
    write_lock documents_lock(documents_mutex_);

    if (documents_.find(doc) == documents_.end()) {
        return;
    }

    documents_.erase(doc);

    write_lock index_write_lock(index_mutex_);

    for (auto it = index_.begin(); it != index_.end();) {
        write_lock word_lock(index_word_mutexes_.at(it->first));

        it->second.erase(doc);

        if (it->second.empty()) {
            it = index_.erase(it);

            continue;
        }

        ++it;
    }
}

void inverted_index::clear() {
    write_lock index_write_lock(index_mutex_);

    for (auto& [word, docs] : index_) {
        write_lock word_lock(index_word_mutexes_.at(word));

        docs.clear();
    }

    index_.clear();
    documents_.clear();
}

void inverted_index::save_as_json(const string& file_path) const {
    json j;
    read_lock index_read_lock(index_mutex_);

    for (const auto& [word, docs] : index_) {
        read_lock word_lock(index_word_mutexes_.at(word));
        string key = wstring_convert<codecvt_utf8<wchar_t>>().to_bytes(word);

        j[key] = docs;
    }

    ofstream file(file_path);

    file << j.dump(2);
}

document inverted_index::read(const std::unordered_set<word>& words) const {
    std::unordered_map<document, int> doc_count;
    document most_relevant_doc;
    int max_count = 0;

    {
        read_lock index_read_lock(index_mutex_);
        for (const auto& w : words) {
            auto it = index_.find(w);
            if (it != index_.end()) {
                read_lock word_lock(index_word_mutexes_.at(w));
                for (const auto& doc : it->second) {
                    int count = ++doc_count[doc];

                    if (count > max_count) {
                        max_count = count;
                        most_relevant_doc = doc;
                    }
                }
            }
        }
    }

    return most_relevant_doc;
}

shared_mutex& inverted_index::get_word_mutex(const word& word) {
    write_lock word_mutexes_lock(index_word_mutexes_mutex_);

    return index_word_mutexes_[word];
}

void inverted_index::add_documents_to_word(const word& word, const documents& docs) {
    {
        write_lock index_write_lock(index_mutex_);

        if (!index_.contains(word)) {
            index_[word] = documents();
        }

        write_lock word_lock(get_word_mutex(word));

        for (const auto& doc : docs) {
            index_[word].insert(doc);
            unique_lock documents_lock(documents_mutex_);
            documents_.insert(doc);
        }
    }
}
