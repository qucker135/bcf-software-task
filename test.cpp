#include <gtest/gtest.h>

TEST(UnitTest, Test1){
    EXPECT_TRUE(true);
}

TEST(UnitTest, Test2){
    EXPECT_FALSE(false);
}

TEST(UnitTest, Test3){
    EXPECT_LT(3, 5);
}

int main(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}