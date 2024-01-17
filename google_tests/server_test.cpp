#include <gtest/gtest.h>
#include "server.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

using std::cout;
using std::endl;

class ServerTest : public ::testing::Test {
protected:
    server* test_server;
    thread_pool* pool;
    inverted_index* index;
    document_parser* parser;
    processing_type type;
    fs::path stop_words_file = "/home/mykyta/uni/PC/inverted-index/data/stop_words.txt";

    ServerTest() {
        parser = new document_parser();

        parser->add_stop_words(stop_words_file);
    }

    void SetUp() override {
        pool = new thread_pool(4);
        index = new inverted_index();
    }

    void TearDown() override {
        delete test_server;
    }
};

TEST_F(ServerTest, WORD_FILES_250) {
    type = WORD_FILES;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test/neg";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_files_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILES_250): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILE_250) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test/neg";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILE_250): " << duration << "ms" << endl;
}

TEST_F(ServerTest, INDEX_250) {
    type = INDEX;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test/neg";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (INDEX_250): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILES_500) {
    type = WORD_FILES;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_files_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILES_500): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILE_500) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILE_500): " << duration << "ms" << endl;
}

TEST_F(ServerTest, INDEX_500) {
    type = INDEX;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/test";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (INDEX_500): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILES_1000) {
    type = WORD_FILES;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/train";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_files_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILES_1000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILE_1000) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/train";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILE_1000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, INDEX_1000) {
    type = INDEX;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset/train";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (INDEX_1000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILES_2000) {
    type = WORD_FILES;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_files_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILES_2000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, WORD_FILE_2000) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (WORD_FILE_2000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, INDEX_2000) {
    type = INDEX;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/index.json";

    const auto duration = test_server->run(input_dir, output_file);

    cout << "Duration (INDEX_2000): " << duration << "ms" << endl;
}

TEST_F(ServerTest, READ_TEST_NEG) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read("Mykyta Krainik");
    auto file2 = test_server->read("'Northfork' is what is wrong with indie films");

    EXPECT_EQ(file1, "/home/mykyta/uni/PC/inverted-index/data/dataset/test/neg/document1.txt");
    EXPECT_EQ(file2, "/home/mykyta/uni/PC/inverted-index/data/dataset/test/neg/3001_1.txt");
}

TEST_F(ServerTest, READ_TEST_POS) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read("King of the Underworld");
    auto file2 = test_server->read(
            "Radio City Music Hall The film's Theodore is just the \"Music Hall.\""
    );

    EXPECT_EQ(file1, "/home/mykyta/uni/PC/inverted-index/data/dataset/test/pos/3001_7.txt");
    EXPECT_EQ(file2, "/home/mykyta/uni/PC/inverted-index/data/dataset/test/pos/3053_8.txt");
}

TEST_F(ServerTest, READ_TRAIN_NEG) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read("competitive world of step-dancing");
    auto file2 = test_server->read(
            "MST3K The Hands of Fate think twice about seeing this one"
    );

    EXPECT_EQ(file1, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/neg/3035_2.txt");
    EXPECT_EQ(file2, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/neg/3081_2.txt");
}

TEST_F(ServerTest, READ_TRAIN_POS) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read(
            "Demonicus a video game Horror and sword fight freaks"
    );
    auto file2 = test_server->read(
            "Boston Tivo House for my intellectual-mystery-jackass Legal vendetta"
    );

    EXPECT_EQ(file1, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/pos/3047_7.txt");
    EXPECT_EQ(file2, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/pos/3095_10.txt");
}

TEST_F(ServerTest, READ_TRAIN_UNSUP) {
    type = WORD_FILE;
    test_server = new server(pool, index, parser, type);

    fs::path input_dir = "/home/mykyta/uni/PC/inverted-index/data/dataset";
    fs::path output_file = "/home/mykyta/uni/PC/inverted-index/data/word_file_index.json";

    test_server->run(input_dir, output_file);

    auto file1 = test_server->read(
            "I would say for making a movie during the writer's strike low budget filmmakers"
    );
    auto file2 = test_server->read("supposed family killing each other Huckleberry Fin");
    auto file3 = test_server->read("perfect movie projecting 'Desu'");
    auto file4 = test_server->read("J.D Chakraverthy evokes much emotions");

    EXPECT_EQ(file1, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/unsup/12008_0.txt");
    EXPECT_EQ(file2, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/unsup/12022_0.txt");
    EXPECT_EQ(file3, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/unsup/12037_0.txt");
    EXPECT_EQ(file4, "/home/mykyta/uni/PC/inverted-index/data/dataset/train/unsup/12039_0.txt");
}
