#include <iostream>
#include <filesystem>
#include "server.h"
#include "document_parser.h"
#include "inverted_index.h"
#include "thread_pool.h"

namespace fs = std::filesystem;

int main() {
    auto* pool = new thread_pool(4);
    auto* index = new inverted_index();
    auto* parser = new document_parser();
    processing_type type = WORD_FILE;
    auto* test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read("King of the Underworld");
    auto file2 = test_server->read(
            "Radio City Music Hall The film's Theodore is just the \"Music Hall.\""
    );

    std::cout << "File1: " << file1 << std::endl;
    std::cout << "File2: " << file2 << std::endl;

    return 0;
}
