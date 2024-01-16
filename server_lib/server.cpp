#include "server.h"
#include <fstream>
#include <chrono>
#include <iostream>

using std::ifstream;
using std::cout;
using std::endl;

namespace ch = std::chrono;

server::server(
        thread_pool *pool,
        inverted_index *index,
        document_parser *parser,
        processing_type& type
) {
    pool_ = pool;
    index_ = index;
    parser_ = parser;
    type_ = type;
}

server::~server() {
    delete pool_;
    delete index_;
    delete parser_;
}

void server::run(const fs::path &input_dir, const fs::path& output_file) {
    if (!fs::exists(input_dir)) {
        throw std::runtime_error("Input directory does not exist");
    }

    auto start = ch::high_resolution_clock::now();

    pool_->add_task([this, input_dir] {
        try {
            process_dir(input_dir);
        } catch (std::runtime_error& e) {
            return false;
        }

        return true;
    });

    pool_->wait_all();

    auto end = ch::high_resolution_clock::now();
    auto duration = ch::duration_cast<ch::milliseconds>(end - start);

    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    save_to_json(output_file);
}

void server::process_dir(const fs::path &input_dir) {
    vector<fs::path> input_files;

    for (const auto& entry : fs::directory_iterator(input_dir)) {
        if (fs::is_directory(entry)) {
            pool_->add_task([this, entry] {
                try {
                    process_dir(entry);
                } catch (std::runtime_error& e) {
                    return false;
                }

                return true;
            });
        } else if (fs::is_regular_file(entry)) {
            switch (type_) {
                case WORD_FILE:
                    word_file_task(entry);
                    break;

                case WORD_FILES:
                case INDEX:
                    input_files.push_back(entry);
                    break;
            }
        }
    }

    if (type_ == WORD_FILES) {
        word_files_task(input_files);
    } else if (type_ == INDEX) {
        index_task(input_files);
    }
}

void server::save_to_json(const fs::path &output_dir) {
    index_->save_as_json(output_dir);
}

document server::read(const string& content) const {
    wstring wcontent = document_parser::string_to_wstring(content);
    unordered_set<word> words = parser_->parse_words(wcontent);

    return index_->read(words);
}

void server::word_file_task(const fs::path &input_file) {
    pool_->add_task([this, input_file] {
        auto words = parser_->parse_document(input_file);

        for (const auto& word : words) {
            index_->add(word, input_file);
        }

        return true;
    });
}

void server::word_files_task(const vector<fs::path> &input_files) {
    pool_->add_task([this, input_files] {
        inv_index index;

        for (const auto& file : input_files) {
            auto words = parser_->parse_document(file);

            for (const auto& word : words) {
                index[word].insert(file);
            }
        }

        for (const auto& [word, docs] : index) {
            index_->add(word, docs);
        }

        return true;
    });
}

void server::index_task(const vector<fs::path> &input_files) {
    pool_->add_task([this, input_files] {
        inv_index index;

        for (const auto& file : input_files) {
            auto words = parser_->parse_document(file);

            for (const auto& word : words) {
                index[word].insert(file);
            }
        }

        index_->add(index);

        return true;
    });
}

void server::parse_dir_task(const fs::path &input_dir) {

}

void server::parse_file_task(const fs::path &input_file) {

}
