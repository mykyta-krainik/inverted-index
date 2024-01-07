#include <iostream>
#include <fstream>
#include "inverted_index.h"

using std::ifstream;

int main() {
    word test_word = L"example";
    document test_doc = "doc1";
    auto* index = new inverted_index();

    index->add(test_word, test_doc);
    index->add(test_word, "doc2");
    index->add(test_word, "doc3");
    index->add(L"example2", "doc1");

    string file_path = "/home/mykyta/uni/PC/inverted-index/data/test_index.json";

    index->save_as_json(file_path);

    ifstream file(file_path);

    file.close();

    return 0;
}
