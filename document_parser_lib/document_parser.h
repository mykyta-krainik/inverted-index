#ifndef INVERTED_INDEX_LIB_FILE_PARSER_H
#define INVERTED_INDEX_LIB_FILE_PARSER_H

#include "english_stem.h"
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::wstring;

using document_path = string;
using words = vector<wstring>;

class document_parser {
public:
    explicit document_parser();
    ~document_parser();

    words parse_document(const document_path& path);

private:
    stemming::english_stem<>* stem_english;

    void stem_word(wstring& word);

    static std::basic_string<wchar_t> string_to_wstring(const string& str);
};

#endif
