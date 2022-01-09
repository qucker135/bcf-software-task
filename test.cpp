#include <gtest/gtest.h>
#include "staticstics.hpp"
#include "display.hpp"
#include "thread_pool.hpp"

TEST(UnitTest, NonExistentFile){
    ThreadPool tpTest(1);
    Statistics sTested = DisplayDirTreeUsingThreadPool("./test-dir/dir1/nonexistentfile.txt", tpTest);
    EXPECT_EQ(sTested.files, 0);
    EXPECT_EQ(sTested.non_empty_lines, 0);
    EXPECT_EQ(sTested.empty_lines, 0);
    EXPECT_EQ(sTested.words, 0);
    EXPECT_EQ(sTested.letters, 0);
}

TEST(UnitTest, ExistingFile){
    ThreadPool tpTest(1);
    Statistics sTested = DisplayDirTreeUsingThreadPool("./test-dir/dir1/outoftouch.txt", tpTest);
    EXPECT_EQ(sTested.files, 1);
    EXPECT_EQ(sTested.non_empty_lines, 4);
    EXPECT_EQ(sTested.empty_lines, 1);
    EXPECT_EQ(sTested.words, 4);
    EXPECT_EQ(sTested.letters, 25);
}

TEST(UnitTest, ExistingDirectory){
    ThreadPool tpTest(5);
    Statistics sTested = DisplayDirTreeUsingThreadPool("./test-dir", tpTest);
    EXPECT_EQ(sTested.files, 5);
    EXPECT_EQ(sTested.non_empty_lines, 6);
    EXPECT_EQ(sTested.empty_lines, 2);
    EXPECT_EQ(sTested.words, 8);
    EXPECT_EQ(sTested.letters, 47);
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}