#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <mutex>
#include <shared_mutex>

using std::shared_mutex;
using std::unique_lock;
using std::shared_lock;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::wstring;

using word = wstring;
using document = string;
using documents = unordered_set<document>;
using inv_index = unordered_map<word, documents>;
using read_lock = shared_lock<shared_mutex>;
using write_lock = unique_lock<shared_mutex>;

class inverted_index {
public:
    inverted_index();
    ~inverted_index();

    void add(const word& word, const document& document);
    void add(const word& word, const documents& docs);
    void add(const inv_index& idx);
    const documents& find(const word& word) const;
    bool contains(const word& word) const;
    void remove_word(const word& word);
    void remove_document_from_all_records(const document& doc);
    void clear();
    void save_as_json(const string& file_path) const;
    document read(const unordered_set<word>& words) const;

private:
    inv_index index_;
    mutable shared_mutex index_mutex_;
    mutable unordered_map<word, shared_mutex> index_word_mutexes_;
    mutable shared_mutex index_word_mutexes_mutex_;

    documents documents_;
    mutable shared_mutex documents_mutex_;

    shared_mutex& get_word_mutex(const word& word);
    void add_documents_to_word(const word& word, const documents& docs);
};

#endif
