#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include "inverted_index.h"

using std::ifstream;
using std::filesystem::remove;
using std::thread;
using std::vector;
using std::to_string;

class InvertedIndexTest : public ::testing::Test {
protected:
    inverted_index* index;
    
    void SetUp() override {
        index = new inverted_index();
    }

    void TearDown() override {
        delete index;
    }
};

TEST_F(InvertedIndexTest, AddSingleDocument) {
    word test_word = L"example";
    document test_doc = "doc1";

    index->add(test_word, test_doc);

    const auto& docs = index->find(test_word);

    EXPECT_TRUE(docs.contains(test_doc));
}

TEST_F(InvertedIndexTest, AddMultipleDocuments) {
    word test_word = L"example";
    documents test_docs = {"doc1", "doc2"};
    index->add(test_word, test_docs);

    const auto& docs = index->find(test_word);

    for (const auto& doc : test_docs) {
        EXPECT_TRUE(docs.contains(doc));
    }
}

TEST_F(InvertedIndexTest, AddWordToDocumentsMap) {
    inv_index test_idx = {
        {L"word1", {"doc1", "doc2"}},
        {L"word2", {"doc3"}}
    };

    index->add(test_idx);

    for (const auto& [word, docs] : test_idx) {
        const auto& retrieved_docs = index->find(word);

        for (const auto& doc : docs) {
            EXPECT_TRUE(retrieved_docs.contains(doc));
        }
    }
}

TEST_F(InvertedIndexTest, RemoveWord) {
    word test_word = L"example";
    document test_doc = "doc1";

    index->add(test_word, test_doc);

    index->remove_word(test_word);

    EXPECT_FALSE(index->contains(test_word));
}

TEST_F(InvertedIndexTest, RemoveDocumentFromAllRecords) {
    word test_word = L"example";
    document test_doc = "doc1";

    index->add(test_word, test_doc);

    index->remove_document_from_all_records(test_doc);

    const auto& docs = index->find(test_word);

    EXPECT_FALSE(docs.contains(test_doc));
}

TEST_F(InvertedIndexTest, SaveAsJson) {
    word test_word = L"example";
    document test_doc = "doc1";

    index->add(test_word, test_doc);

    string file_path = "{your path}";

    index->save_as_json(file_path);

    ifstream file(file_path);

    EXPECT_TRUE(file.good());

    file.close();
    remove(file_path);
}

TEST_F(InvertedIndexTest, ClearMethodBasicTest) {
    word test_word = L"example";
    document test_doc = "doc1";

    index->add(test_word, test_doc);
    ASSERT_TRUE(index->contains(test_word));

    index->clear();
    EXPECT_FALSE(index->contains(test_word));
    EXPECT_TRUE(index->find(test_word).empty());
}

TEST_F(InvertedIndexTest, NoOverlappingDocuments) {
    index->add(L"word1", "doc1");
    index->add(L"word2", "doc2");

    std::unordered_set<word> words = {L"word1", L"word2"};
    document result = index->read(words);

    EXPECT_EQ(result, "doc2");
}

TEST_F(InvertedIndexTest, SomeOverlappingDocuments) {
    index->add(L"word1", "doc1");
    index->add(L"word1", "doc2");
    index->add(L"word2", "doc2");
    index->add(L"word3", "doc3");

    std::unordered_set<word> words = {L"word1", L"word2", L"word3"};
    document result = index->read(words);

    EXPECT_EQ(result, "doc2");
}

TEST_F(InvertedIndexTest, AllDocumentsOverlapping) {
    index->add(L"word1", "doc1");
    index->add(L"word2", "doc1");
    index->add(L"word3", "doc1");

    std::unordered_set<word> words = {L"word1", L"word2", L"word3"};
    document result = index->read(words);

    EXPECT_EQ(result, "doc1"); // 'doc1' is the only document and appears in all word sets
}

void add_documents(inverted_index& index, const word& w, int num_docs, int start_id) {
    for (int i = 0; i < num_docs; ++i) {
        index.add(w, "doc" + to_string(start_id + i));
    }
}

TEST_F(InvertedIndexTest, ConcurrentAdditionOfDocuments) {
    word test_word = L"concurrent";
    int num_threads = 10;
    int docs_per_thread = 10;
    std::vector<std::thread> threads;

    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(add_documents, std::ref(*index), test_word, docs_per_thread, i * docs_per_thread);
    }

    for (auto& t : threads) {
        t.join();
    }

    const auto& docs = index->find(test_word);
    EXPECT_EQ(docs.size(), num_threads * docs_per_thread);
}

void read_and_write_operations(inverted_index& index, const word& w, const document& doc) {
    index.add(w, doc);
    index.contains(w);
    index.find(w);
    index.remove_document_from_all_records(doc);
}

TEST_F(InvertedIndexTest, ConcurrentReadsAndWrites) {
    word test_word = L"multithread";
    document test_doc = "doc_multi";
    int num_threads = 10;
    std::vector<std::thread> threads;

    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(read_and_write_operations, std::ref(*index), test_word, test_doc);
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_FALSE(index->contains(test_word));
}

void remove_words(inverted_index& index, const word& w) {
    index.remove_word(w);
}

TEST_F(InvertedIndexTest, ConcurrentRemovalOfWords) {
    word test_word = L"to_remove";
    document test_doc = "doc_remove";
    index->add(test_word, test_doc);

    int num_threads = 10;
    vector<thread> threads;

    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(remove_words, std::ref(*index), test_word);
    }

    for (auto& t : threads) {
        t.join();
    }

    EXPECT_FALSE(index->contains(test_word));
}

void perform_operations(inverted_index& index, int id) {
    word test_word = L"word" + std::to_wstring(id);
    document test_doc = "doc" + std::to_string(id);

    index.add(test_word, test_doc);

    if (id % 2 == 0) {
        index.remove_word(test_word);
    }
}

TEST_F(InvertedIndexTest, ClearMethodThreadSafetyTest) {
    const int num_threads = 10;
    vector<thread> threads;

    threads.reserve(num_threads);

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(perform_operations, std::ref(*index), i);
    }

    // Let the threads do some work
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Clear the index in the middle of operations
    index->clear();

    for (auto& t : threads) {
        t.join();
    }

    // Verify that the index is cleared
    EXPECT_TRUE(index->find(L"word5").empty());
    EXPECT_FALSE(index->contains(L"word5"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}