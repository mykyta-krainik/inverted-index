#ifndef INVERTED_INDEX_LIB_SERVER_H
#define INVERTED_INDEX_LIB_SERVER_H

#include "thread_pool.h"
#include "inverted_index.h"
#include "document_parser.h"
#include "../enums_lib/processing_type.h"

#include <string>
#include <filesystem>
#include <vector>
#include <chrono>

namespace fs = std::filesystem;
namespace ch = std::chrono;

using std::string;
using std::vector;

class server {
public:
    server(
            thread_pool *pool,
            inverted_index *index,
            document_parser *parser,
            processing_type& type
    );
    ~server();
    long int run(const fs::path& input_dir, const fs::path& output_file);
    void save_to_json(const fs::path& output_dir);
    [[nodiscard]] document read(const string& content) const;

private:
    thread_pool *pool_ = nullptr;
    inverted_index *index_ = nullptr;
    document_parser *parser_ = nullptr;
    processing_type type_ = WORD_FILE;

    void process_dir(const fs::path& input_dir);

    void parse_dir_task(const fs::path& input_dir);

    void word_file_task(const fs::path &input_dir);
    void word_files_task(const vector<fs::path> &input_files);
    void index_task(const vector<fs::path> &input_files);
};

#endif
