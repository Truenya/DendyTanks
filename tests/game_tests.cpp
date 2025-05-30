
// game_tests.cpp
// Unit tests for the Game class

#include <gtest/gtest.h>
#include "Entities/GameObjects/GameObject.h"
#include "Entities/GameObjects/Position.h"

// Test Position constructor and accessors
TEST(PositionTest, ConstructorAndAccessors) {
    // Create a position with specific coordinates
    Position pos(5, 10, 0, Position::Direction::RIGHT);

    // Check the values are set correctly
    EXPECT_EQ(pos.x_, 5);
    EXPECT_EQ(pos.y_, 10);
    EXPECT_EQ(pos.z_, 0);
    EXPECT_EQ(pos.direction_, Position::Direction::RIGHT);
}

// Test Direction enum values
TEST(PositionTest, DirectionValues) {
    // Check that the direction enum has all expected values
    EXPECT_EQ(Position::Direction::TOP, 0);
    EXPECT_EQ(Position::Direction::BOTTOM, 1);
    EXPECT_EQ(Position::Direction::LEFT, 2);
    EXPECT_EQ(Position::Direction::RIGHT, 3);
}

// Test Position equality operator
TEST(PositionTest, EqualityOperator) {
    // Create two identical positions
    Position pos1(5, 10, 0, Position::Direction::TOP);
    Position pos2(5, 10, 0, Position::Direction::TOP);

    // Check that they are equal
    EXPECT_EQ(pos1, pos2);

    // Change one position and check inequality
    pos2.x_ = 6;
    EXPECT_NE(pos1, pos2);
}

// Test Position direction change
TEST(PositionTest, DirectionChange) {
    // Create a position with initial direction
    Position pos(5, 10, 0, Position::Direction::TOP);

    // Change the direction and check
    pos.direction_ = Position::Direction::RIGHT;
    EXPECT_EQ(pos.direction_, Position::Direction::RIGHT);
}

