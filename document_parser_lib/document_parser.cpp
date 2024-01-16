#include "document_parser.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

using std::ifstream;
using std::stringstream;
using std::iswalpha;
using std::towlower;
using std::cerr;
using std::endl;
using std::runtime_error;
using std::mbstowcs;

document_parser::document_parser() {
    stem_english = new stemming::english_stem<>();
}

document_parser::~document_parser() {
    delete stem_english;
}

words document_parser::parse_document(const document_path &path) {
    try {
        ifstream file(path);

        if (!file.is_open()) {
            throw runtime_error("Cannot open file" + path);
        }

        stringstream buffer;

        buffer << file.rdbuf();

        file.close();

        wstring content = string_to_wstring(buffer.str());

        return parse_words(content);
    } catch (runtime_error& e) {
        cerr << e.what() << endl;

        return {};
    }
}

void document_parser::stem_word(wstring &word) {
    stem_english->operator()(word);
}

basic_string<wchar_t> document_parser::string_to_wstring(const string &str) {
    if (str.empty()) {
        return {};
    }

    size_t size_needed = mbstowcs(nullptr, str.c_str(), 0) + 1;
    wstring wstr_to(size_needed, 0);

    mbstowcs(&wstr_to[0], str.c_str(), size_needed);

    return wstr_to;
}

bool document_parser::add_stop_words(const fs::path &path) {
    try {
        ifstream file(path);

        if (!file.is_open()) {
            throw runtime_error("Cannot open file" + path.string());
        }

        stringstream buffer;

        buffer << file.rdbuf();

        file.close();

        wstring content = string_to_wstring(buffer.str());

        wstring word;

        for (auto& it : content) {
            if (iswalpha(it)) {
                word += towlower(it);

                continue;
            }

            if (!word.empty()) {
                stop_words_.insert(word);
                word.clear();
            }
        }
    } catch (runtime_error& e) {
        cerr << e.what() << endl;

        return false;
    }

    return true;
}

words document_parser::parse_words(const wstring &content) {
    words result;
    wstring word;

    for (auto& it : content) {
        if (iswalpha(it)) {
            word += towlower(it);

            continue;
        }

        if (word.empty()) {
            continue;
        }

        stem_word(word);

        if (!stop_words_.contains(word)) {
            result.insert(word);
        }

        word.clear();
    }

    return result;
}
