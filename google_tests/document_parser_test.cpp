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
    document_path valid_path = "{your path}";
    words expected_words = {/* your words */};
    words parsed_words = parser->parse_document(valid_path);

    ASSERT_EQ(parsed_words, expected_words);
}

TEST_F(DocumentParserTest, HandlesInvalidDocumentPath) {
    document_path invalid_path = "{your path}";
    words parsed_words = parser->parse_document(invalid_path);

    ASSERT_TRUE(parsed_words.empty());
}

TEST_F(DocumentParserTest, HandlesEmptyDocument) {
    document_path empty_path = "{your path}";
    words parsed_words = parser->parse_document(empty_path);

    ASSERT_TRUE(parsed_words.empty());
}
