#include <gtest/gtest.h>
#include "document_parser.h"
#include <stdexcept>

using std::runtime_error;

class DocumentParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        parser = new document_parser();
    }

    void TearDown() override {
        delete parser;
    }

    document_parser* parser = nullptr;
};

TEST_F(DocumentParserTest, ParsesValidDocument) {
    document_path valid_path = "/home/mykyta/uni/PC/inverted-index/data/test_files/document1.txt";
    words expected_words = {L"hi", L"i", L"am", L"connect", L"mykyta", L"krainik"};
    words parsed_words = parser->parse_document(valid_path);

    ASSERT_EQ(parsed_words, expected_words);
}

TEST_F(DocumentParserTest, HandlesInvalidDocumentPath) {
    document_path invalid_path = "/home/mykyta/uni/PC/inverted-index/data/test_files/not_exist.txt";
    words parsed_words = parser->parse_document(invalid_path);

    ASSERT_TRUE(parsed_words.empty());
}

TEST_F(DocumentParserTest, HandlesEmptyDocument) {
    document_path empty_path = "/home/mykyta/uni/PC/inverted-index/data/test_files/empty.txt";
    words parsed_words = parser->parse_document(empty_path);

    ASSERT_TRUE(parsed_words.empty());
}
