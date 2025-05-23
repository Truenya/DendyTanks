#include <gtest/gtest.h>
#include "../Entities/GameObjects/GameObject.h"

// Test fixture for GameObject
class GameObjectTest : public ::testing::Test {
protected:
    // Default GameObject
    GameObject defaultObj;
    
    // GameObject with specific position and type
    Position pos1{5, 10, 0, Position::Direction::RIGHT};
    GameObject playerObj;
    
    void SetUp() override {
        // Initialize the player object
        playerObj = GameObject(pos1, GameObject::Type::PLAYER);
    }
    
    void TearDown() override {
        // Cleanup after each test
    }
};

// Test default constructor
TEST_F(GameObjectTest, DefaultConstructor) {
    // Check type
    EXPECT_EQ(defaultObj.type_, GameObject::Type::UNDEFINED);
    
    // Check positions
    const Positions& positions = defaultObj.getPositions();
    EXPECT_EQ(positions.prevPos_.x_, -1);
    EXPECT_EQ(positions.prevPos_.y_, -1);
    EXPECT_EQ(positions.prevPos_.z_, 0);
    EXPECT_EQ(positions.prevPos_.direction_, Position::Direction::BOT);
    
    EXPECT_EQ(positions.curPos_.x_, -1);
    EXPECT_EQ(positions.curPos_.y_, -1);
    EXPECT_EQ(positions.curPos_.z_, 0);
    EXPECT_EQ(positions.curPos_.direction_, Position::Direction::BOT);
}

// Test parameterized constructor
TEST_F(GameObjectTest, ParameterizedConstructor) {
    // Check type
    EXPECT_EQ(playerObj.type_, GameObject::Type::PLAYER);
    
    // Check positions
    const Positions& positions = playerObj.getPositions();
    EXPECT_EQ(positions.prevPos_.x_, 5);
    EXPECT_EQ(positions.prevPos_.y_, 10);
    EXPECT_EQ(positions.prevPos_.z_, 0);
    EXPECT_EQ(positions.prevPos_.direction_, Position::Direction::RIGHT);
    
    EXPECT_EQ(positions.curPos_.x_, 5);
    EXPECT_EQ(positions.curPos_.y_, 10);
    EXPECT_EQ(positions.curPos_.z_, 0);
    EXPECT_EQ(positions.curPos_.direction_, Position::Direction::RIGHT);
}

// Test step method
TEST_F(GameObjectTest, StepMethod) {
    // Step the player object (should move in the RIGHT direction)
    playerObj.step();
    
    // Check positions after step
    const Positions& positions = playerObj.getPositions();
    
    // Previous position should be the original position
    EXPECT_EQ(positions.prevPos_.x_, 5);
    EXPECT_EQ(positions.prevPos_.y_, 10);
    EXPECT_EQ(positions.prevPos_.z_, 0);
    EXPECT_EQ(positions.prevPos_.direction_, Position::Direction::RIGHT);
    
    // Current position should have moved one step to the right
    EXPECT_EQ(positions.curPos_.x_, 6);  // x increased by 1 (moved right)
    EXPECT_EQ(positions.curPos_.y_, 10); // y unchanged
    EXPECT_EQ(positions.curPos_.z_, 0);  // z unchanged
    EXPECT_EQ(positions.curPos_.direction_, Position::Direction::RIGHT);
}

// Test rotate method
TEST_F(GameObjectTest, RotateMethod) {
    // Rotate the player object to a different direction
    playerObj.rotate(Position::Direction::TOP);
    
    // Check that the direction has changed
    const Positions& positions = playerObj.getPositions();
    EXPECT_EQ(positions.curPos_.direction_, Position::Direction::TOP);
    
    // Step after rotation should move in the new direction
    playerObj.step();
    
    // Check positions after step
    const Positions& positionsAfterStep = playerObj.getPositions();
    
    // Previous position should be the original position with the new direction
    EXPECT_EQ(positionsAfterStep.prevPos_.x_, 5);
    EXPECT_EQ(positionsAfterStep.prevPos_.y_, 10);
    EXPECT_EQ(positionsAfterStep.prevPos_.z_, 0);
    EXPECT_EQ(positionsAfterStep.prevPos_.direction_, Position::Direction::TOP);
    
    // Current position should have moved one step up
    EXPECT_EQ(positionsAfterStep.curPos_.x_, 5);  // x unchanged
    EXPECT_EQ(positionsAfterStep.curPos_.y_, 9);  // y decreased by 1 (moved up)
    EXPECT_EQ(positionsAfterStep.curPos_.z_, 0);  // z unchanged
    EXPECT_EQ(positionsAfterStep.curPos_.direction_, Position::Direction::TOP);
}

// Test multiple steps
TEST_F(GameObjectTest, MultipleSteps) {
    // Take multiple steps
    playerObj.step();  // First step (RIGHT)
    playerObj.step();  // Second step (RIGHT)
    
    // Check positions after steps
    const Positions& positions = playerObj.getPositions();
    
    // Previous position should be after the first step
    EXPECT_EQ(positions.prevPos_.x_, 6);
    EXPECT_EQ(positions.prevPos_.y_, 10);
    
    // Current position should be after the second step
    EXPECT_EQ(positions.curPos_.x_, 7);
    EXPECT_EQ(positions.curPos_.y_, 10);
}

// Test rotation followed by multiple steps
TEST_F(GameObjectTest, RotateAndMultipleSteps) {
    // Rotate and take multiple steps
    playerObj.rotate(Position::Direction::BOT);
    playerObj.step();  // First step (BOT)
    playerObj.step();  // Second step (BOT)
    
    // Check positions after steps
    const Positions& positions = playerObj.getPositions();
    
    // Previous position should be after the first step
    EXPECT_EQ(positions.prevPos_.x_, 5);
    EXPECT_EQ(positions.prevPos_.y_, 11);
    
    // Current position should be after the second step
    EXPECT_EQ(positions.curPos_.x_, 5);
    EXPECT_EQ(positions.curPos_.y_, 12);
}

// Main function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}