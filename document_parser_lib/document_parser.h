#ifndef INVERTED_INDEX_LIB_FILE_PARSER_H
#define INVERTED_INDEX_LIB_FILE_PARSER_H

#include "english_stem.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

using std::vector;
using std::unordered_set;

using std::string;
using std::wstring;

using document_path = string;
using words = unordered_set<wstring>;
using stop_words = words;

class document_parser {
public:
    explicit document_parser();
    ~document_parser();

    words parse_document(const document_path& path);
    bool add_stop_words(const fs::path &path);
    words parse_words(const wstring& content);
    static std::basic_string<wchar_t> string_to_wstring(const string& str);

private:
    stemming::english_stem<>* stem_english;
    stop_words stop_words_{};

    void stem_word(wstring& word);

};

#endif
