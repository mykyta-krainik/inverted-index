#include "document_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cstdlib>

using std::ifstream;
using std::stringstream;
using std::iswalpha;
using std::towlower;
using std::cerr;
using std::endl;
using std::runtime_error;
using std::locale;
using std::mbstowcs;
using std::basic_string;

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

        wstring content = string_to_wstring(buffer.str());

        words result;
        wstring word;

        for (auto& it : content) {
            if (iswalpha(it)) {
                word += towlower(it);

                continue;
            }

            if (!word.empty()) {
                stem_word(word);
                result.push_back(word);
                word.clear();
            }
        }

        return result;
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
    wstring wstrTo(size_needed, 0);
    mbstowcs(&wstrTo[0], str.c_str(), size_needed);

    return wstrTo;
}
