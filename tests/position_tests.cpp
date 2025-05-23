#include <gtest/gtest.h>
#include "../Entities/GameObjects/Position.h"

// Test fixture for Position
class PositionTest : public ::testing::Test {
protected:
    Position pos1;
    Position pos2;
    Position pos3;

    void SetUp() override {
        // Initialize positions for testing
        pos1.x_ = 5;
        pos1.y_ = 10;
        pos1.z_ = 0;
        pos1.direction_ = Position::Direction::TOP;
        
        pos2.x_ = 3;
        pos2.y_ = 7;
        pos2.z_ = 0;
        pos2.direction_ = Position::Direction::RIGHT;
        
        pos3.x_ = -5;
        pos3.y_ = -5;
        pos3.z_ = 0;
        pos3.direction_ = Position::Direction::LEFT;
    }
};

// Test equality operators
TEST_F(PositionTest, EqualityOperators) {
    // Test equality
    Position sameAsPos1;
    sameAsPos1.x_ = 5;
    sameAsPos1.y_ = 10;
    sameAsPos1.z_ = 0;
    sameAsPos1.direction_ = Position::Direction::TOP;
    
    EXPECT_TRUE(pos1 == sameAsPos1);
    EXPECT_FALSE(pos1 == pos2);
    
    // Test inequality
    EXPECT_TRUE(pos1 != pos2);
    
    Position sameAsPos1DiffDirection;
    sameAsPos1DiffDirection.x_ = 5;
    sameAsPos1DiffDirection.y_ = 10;
    sameAsPos1DiffDirection.z_ = 0;
    sameAsPos1DiffDirection.direction_ = Position::Direction::BOT;
    
    EXPECT_FALSE(pos1 != sameAsPos1DiffDirection); // Direction is not considered in equality
}

// Test addition operators
TEST_F(PositionTest, AdditionOperators) {
    // Test addition
    Position result = pos1 + pos2;
    EXPECT_EQ(result.x_, 8);
    EXPECT_EQ(result.y_, 17);
    EXPECT_EQ(result.z_, 0);
    EXPECT_EQ(result.direction_, Position::Direction::RIGHT);
    
    // Test addition assignment
    Position temp = pos1;
    temp += pos2;
    EXPECT_EQ(temp.x_, 8);
    EXPECT_EQ(temp.y_, 17);
    EXPECT_EQ(temp.z_, 0);
    EXPECT_EQ(temp.direction_, Position::Direction::RIGHT);
}

// Test subtraction operators
TEST_F(PositionTest, SubtractionOperators) {
    // Test subtraction
    Position result = pos1 - pos2;
    EXPECT_EQ(result.x_, 2);
    EXPECT_EQ(result.y_, 3);
    EXPECT_EQ(result.z_, 0);
    EXPECT_EQ(result.direction_, Position::Direction::RIGHT);
    
    // Test subtraction assignment
    Position temp = pos1;
    temp -= pos2;
    EXPECT_EQ(temp.x_, 2);
    EXPECT_EQ(temp.y_, 3);
    EXPECT_EQ(temp.z_, 0);
    EXPECT_EQ(temp.direction_, Position::Direction::RIGHT);
    
    // Test unary minus
    Position negated = -pos1;
    EXPECT_EQ(negated.x_, -5);
    EXPECT_EQ(negated.y_, -10);
    EXPECT_EQ(negated.z_, 0);
    EXPECT_EQ(negated.direction_, Position::Direction::BOT); // TOP becomes BOT
}

// Test comparison operators
TEST_F(PositionTest, ComparisonOperators) {
    EXPECT_TRUE(pos2 < pos1);
    EXPECT_TRUE(pos1 > pos2);
    EXPECT_TRUE(pos2 <= pos1);
    EXPECT_TRUE(pos1 >= pos2);
    
    // Test with equal x but different y
    Position pos4;
    pos4.x_ = 5;
    pos4.y_ = 8;
    pos4.z_ = 0;
    
    EXPECT_TRUE(pos4 < pos1);
    EXPECT_TRUE(pos1 > pos4);
}

// Test isValid method
TEST_F(PositionTest, IsValidMethod) {
    // Valid positions
    EXPECT_TRUE(pos1.isValid());
    EXPECT_TRUE(pos2.isValid());
    
    // Invalid positions
    EXPECT_FALSE(pos3.isValid()); // Negative coordinates
    
    // Test boundary conditions
    Position boundary1;
    boundary1.x_ = 0;
    boundary1.y_ = 0;
    boundary1.z_ = 0;
    EXPECT_TRUE(boundary1.isValid());
    
    Position boundary2;
    boundary2.x_ = 999;
    boundary2.y_ = 999;
    boundary2.z_ = 999;
    EXPECT_TRUE(boundary2.isValid());
    
    Position invalid1;
    invalid1.x_ = -1;
    invalid1.y_ = 10;
    invalid1.z_ = 0;
    EXPECT_FALSE(invalid1.isValid());
    
    Position invalid2;
    invalid2.x_ = 1000;
    invalid2.y_ = 10;
    invalid2.z_ = 0;
    EXPECT_FALSE(invalid2.isValid());
}

// Test isValidByWorldSize method
TEST_F(PositionTest, IsValidByWorldSizeMethod) {
    Position worldSize;
    worldSize.x_ = 20;
    worldSize.y_ = 20;
    worldSize.z_ = 0;
    
    // Valid positions
    EXPECT_TRUE(pos1.isValidByWorldSize(worldSize));
    EXPECT_TRUE(pos2.isValidByWorldSize(worldSize));
    
    // Invalid positions
    Position outsideWorld;
    outsideWorld.x_ = 25;
    outsideWorld.y_ = 10;
    outsideWorld.z_ = 0;
    EXPECT_FALSE(outsideWorld.isValidByWorldSize(worldSize));
    
    Position atBoundary;
    atBoundary.x_ = 19;
    atBoundary.y_ = 19;
    atBoundary.z_ = 0;
    EXPECT_TRUE(atBoundary.isValidByWorldSize(worldSize));
    
    Position beyondBoundary;
    beyondBoundary.x_ = 20;
    beyondBoundary.y_ = 19;
    beyondBoundary.z_ = 0;
    EXPECT_FALSE(beyondBoundary.isValidByWorldSize(worldSize));
}

// Test directionsTo method
TEST_F(PositionTest, DirectionsToMethod) {
    // Test horizontal direction
    Position right;
    right.x_ = 10;
    right.y_ = 10;
    right.z_ = 0;
    auto directions1 = pos1.directionsTo(right);
    EXPECT_EQ(directions1.first, Position::Direction::RIGHT);
    EXPECT_EQ(directions1.second, Position::Direction::EQUAL);
    
    // Test vertical direction
    Position below;
    below.x_ = 5;
    below.y_ = 15;
    below.z_ = 0;
    auto directions2 = pos1.directionsTo(below);
    EXPECT_EQ(directions2.first, Position::Direction::EQUAL);
    EXPECT_EQ(directions2.second, Position::Direction::BOT);
    
    // Test diagonal direction
    Position diagonal;
    diagonal.x_ = 10;
    diagonal.y_ = 15;
    diagonal.z_ = 0;
    auto directions3 = pos1.directionsTo(diagonal);
    EXPECT_EQ(directions3.first, Position::Direction::RIGHT);
    EXPECT_EQ(directions3.second, Position::Direction::BOT);
}

// Test stepInDirection method
TEST_F(PositionTest, StepInDirectionMethod) {
    // Test step in TOP direction
    Position topPos;
    topPos.x_ = 5;
    topPos.y_ = 10;
    topPos.z_ = 0;
    topPos.direction_ = Position::Direction::TOP;
    topPos.stepInDirection();
    EXPECT_EQ(topPos.x_, 5);
    EXPECT_EQ(topPos.y_, 9);
    
    // Test step in RIGHT direction
    Position rightPos;
    rightPos.x_ = 5;
    rightPos.y_ = 10;
    rightPos.z_ = 0;
    rightPos.direction_ = Position::Direction::RIGHT;
    rightPos.stepInDirection();
    EXPECT_EQ(rightPos.x_, 6);
    EXPECT_EQ(rightPos.y_, 10);
    
    // Test step in inverse direction
    Position leftPos;
    leftPos.x_ = 5;
    leftPos.y_ = 10;
    leftPos.z_ = 0;
    leftPos.direction_ = Position::Direction::LEFT;
    leftPos.stepInDirection(true); // Inverse = true, so should go RIGHT
    EXPECT_EQ(leftPos.x_, 6);
    EXPECT_EQ(leftPos.y_, 10);
}

// Test reverseDirection method
TEST_F(PositionTest, ReverseDirectionMethod) {
    Position topPos;
    topPos.x_ = 0;
    topPos.y_ = 0;
    topPos.z_ = 0;
    topPos.direction_ = Position::Direction::TOP;
    topPos.reverseDirection();
    EXPECT_EQ(topPos.direction_, Position::Direction::BOT);
    
    Position rightPos;
    rightPos.x_ = 0;
    rightPos.y_ = 0;
    rightPos.z_ = 0;
    rightPos.direction_ = Position::Direction::RIGHT;
    rightPos.reverseDirection();
    EXPECT_EQ(rightPos.direction_, Position::Direction::LEFT);
    
    Position leftPos;
    leftPos.x_ = 0;
    leftPos.y_ = 0;
    leftPos.z_ = 0;
    leftPos.direction_ = Position::Direction::LEFT;
    leftPos.reverseDirection();
    EXPECT_EQ(leftPos.direction_, Position::Direction::RIGHT);
    
    Position botPos;
    botPos.x_ = 0;
    botPos.y_ = 0;
    botPos.z_ = 0;
    botPos.direction_ = Position::Direction::BOT;
    botPos.reverseDirection();
    EXPECT_EQ(botPos.direction_, Position::Direction::TOP);
}

// Test rotateDirection method
TEST_F(PositionTest, RotateDirectionMethod) {
    Position topPos;
    topPos.x_ = 0;
    topPos.y_ = 0;
    topPos.z_ = 0;
    topPos.direction_ = Position::Direction::TOP;
    topPos.rotateDirection();
    EXPECT_EQ(topPos.direction_, Position::Direction::RIGHT);
    
    Position rightPos;
    rightPos.x_ = 0;
    rightPos.y_ = 0;
    rightPos.z_ = 0;
    rightPos.direction_ = Position::Direction::RIGHT;
    rightPos.rotateDirection();
    EXPECT_EQ(rightPos.direction_, Position::Direction::BOT);
    
    Position botPos;
    botPos.x_ = 0;
    botPos.y_ = 0;
    botPos.z_ = 0;
    botPos.direction_ = Position::Direction::BOT;
    botPos.rotateDirection();
    EXPECT_EQ(botPos.direction_, Position::Direction::LEFT);
    
    Position leftPos;
    leftPos.x_ = 0;
    leftPos.y_ = 0;
    leftPos.z_ = 0;
    leftPos.direction_ = Position::Direction::LEFT;
    leftPos.rotateDirection();
    EXPECT_EQ(leftPos.direction_, Position::Direction::TOP);
}

// Test bool operator
TEST_F(PositionTest, BoolOperator) {
    EXPECT_TRUE(bool(pos1));
    EXPECT_TRUE(bool(pos2));
    EXPECT_FALSE(bool(pos3)); // Invalid position
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}