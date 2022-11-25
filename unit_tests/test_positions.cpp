#include "Position.h"
#include <gtest/gtest.h>

TEST(POSITION, PLUS_EQ){
    Position A{3, 4, 5};
    Position B{3, 7, 8};
    Position C{6, 11, 13};
    A += B;
    ASSERT_EQ(A.x_, 6);
    ASSERT_EQ(A.y_, 11);
    ASSERT_EQ(A.z_, 13);
    ASSERT_EQ(A, C);
}

TEST(POSITION, PLUS_EQ_RHS){
    Position A{0, 3, 5};
    A += {0, 2, 1};
    ASSERT_EQ(A.x_, 0);
    ASSERT_EQ(A.y_, 5);
    ASSERT_EQ(A.z_, 6);
}
TEST(POSITION, VALIDATION){
    Position A{-5, -1, 10000,Position::Direction::LEFT};
    Position B{-10, 0, 9000, Position::Direction::BOT};
    Position C{15, 10, 100, Position::Direction::TOP};
    Position D{0, 11, 33, Position::Direction::UNDEFINED};
    
    ASSERT_TRUE(C);
    ASSERT_FALSE(A);
    ASSERT_FALSE(B);
    ASSERT_FALSE(D);
}
TEST(POSITION, EQUALITY){
    Position A{2, 3, 5};
    Position XYZ {2, 3, 5};
    ASSERT_TRUE(A == XYZ);
}

TEST(POSITION, LT){
    Position A{0, 1, 2};
    Position B{2, 1, 0};
    ASSERT_TRUE(A < B);
}

int main(){
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}