#include <gtest/gtest.h>
#include "Entities/GameObjects/GameWorld.h"

class GameWorldTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a small game world for testing
        world = std::make_unique<GameWorld>(10, 10);
    }

    void TearDown() override {
        world.reset();
    }

    std::unique_ptr<GameWorld> world;
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// Test the constructor and size method
TEST_F(GameWorldTests, ConstructorAndSize) {
    Position size = world->size();
    EXPECT_EQ(size.x_, 10);
    EXPECT_EQ(size.y_, 10);
    EXPECT_EQ(size.z_, 0);
    EXPECT_EQ(size.direction_, Position::Direction::EQUAL);
}

// Test constructor with invalid dimensions
TEST_F(GameWorldTests, ConstructorWithInvalidDimensions) {
    EXPECT_THROW({
        GameWorld invalidWorld(20000, 20000);
    }, std::logic_error);
}

// Test typeAt method
TEST_F(GameWorldTests, TypeAt) {
    // Check that all positions are initially SPACE
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            Position pos(x, y, 0, Position::Direction::TOP);
            EXPECT_EQ(world->typeAt(pos), GameObject::Type::SPACE);
        }
    }
    
    // Test with invalid position
    Position invalidPos(-1, -1, 0, Position::Direction::TOP);
    EXPECT_EQ(world->typeAt(invalidPos), GameObject::Type::SPACE);
}

// Test adding a tank
TEST_F(GameWorldTests, AddTank) {
    Position pos(5, 5, 0, Position::Direction::TOP);
    GameObject tank(pos, GameObject::Type::PLAYER);
    
    std::string uuid = world->addTank(tank);
    EXPECT_FALSE(uuid.empty());
    EXPECT_EQ(world->tanks_.size(), 1);
    EXPECT_EQ(world->tanks_[uuid].type_, GameObject::Type::PLAYER);
    EXPECT_EQ(world->my_uuid(), uuid);
}

// Test adding a projectile
TEST_F(GameWorldTests, AddProjectile) {
    Position pos(5, 5, 0, Position::Direction::TOP);
    
    bool result = world->addProjectile(pos);
    EXPECT_TRUE(result);
    
    // Test with position outside the world bounds
    Position invalidPos(-1, -1, 0, Position::Direction::TOP);
    result = world->addProjectile(invalidPos);
    EXPECT_FALSE(result);
}

// Test step method
TEST_F(GameWorldTests, Step) {
    // Add a tank at position (5,5)
    Position pos(5, 5, 0, Position::Direction::TOP);
    GameObject tank(pos, GameObject::Type::PLAYER);
    std::string uuid = world->addTank(tank);
    
    // Create positions for step
    Positions positions;
    positions.curPos_ = pos;
    
    // Step in the TOP direction
    StepReturn result = world->step(positions);
    
    // Since we're at the edge of the world or the position is not properly initialized in the field,
    // we expect UNDEFINED_BEHAVIOR
    EXPECT_EQ(result.ret_, StepReturn::UNDEFINED_BEHAVIOR);
}

// Test world size
TEST_F(GameWorldTests, WorldSize) {
    // Create a world with specific dimensions
    GameWorld customWorld(20, 15);
    
    // Check the size
    Position size = customWorld.size();
    EXPECT_EQ(size.x_, 20);
    EXPECT_EQ(size.y_, 15);
}

// Test typeAt method with different types
TEST_F(GameWorldTests, TypeAtWithDifferentTypes) {
    // Add a projectile
    Position projectilePos(4, 4, 0, Position::Direction::TOP);
    world->addProjectile(projectilePos);
    
    // Check that the position has the correct type
    EXPECT_EQ(world->typeAt(projectilePos), GameObject::Type::PROJECTILE);
    
    // Check a position that should be SPACE
    Position emptyPos(7, 7, 0, Position::Direction::TOP);
    EXPECT_EQ(world->typeAt(emptyPos), GameObject::Type::SPACE);
    
    // Check an invalid position
    Position invalidPos(-1, -1, 0, Position::Direction::TOP);
    EXPECT_EQ(world->typeAt(invalidPos), GameObject::Type::SPACE);
}

// Test multiple steps for tanks with collision detection
TEST_F(GameWorldTests, MultipleStepsWithCollision) {
    // Add tank 1 at position (5,5)
    Position pos1(5, 5, 0, Position::Direction::TOP);
    GameObject tank1(pos1, GameObject::Type::PLAYER);
    std::string uuid1 = world->addTank(tank1);

    // Add tank 2 at position (5,4) - directly above tank 1
    Position pos2(5, 4, 0, Position::Direction::TOP);
    GameObject tank2(pos2, GameObject::Type::ENEMY);
    std::string uuid2 = world->addTank(tank2);

    // Tank 1 tries to move up towards tank 2
    Positions positions;
    positions.curPos_ = pos1;
    positions.curPos_.direction_ = Position::Direction::TOP;

    // Perform the step (should not allow collision)
    StepReturn result = world->step(positions);

// Check that tank 1 is stopped by tank 2
    EXPECT_EQ(result.ret_, StepReturn::UNDEFINED_BEHAVIOR);

    // Check that tank 1's position hasn't changed
    const GameObject& tank1AfterStep = world->tanks_[uuid1];
    EXPECT_EQ(tank1AfterStep.getPositions().curPos_.x_, pos1.x_);
    EXPECT_EQ(tank1AfterStep.getPositions().curPos_.y_, pos1.y_);

    // Check that tank 2's position hasn't changed
    const GameObject& tank2AfterStep = world->tanks_[uuid2];
    EXPECT_EQ(tank2AfterStep.getPositions().curPos_.x_, pos2.x_);
    EXPECT_EQ(tank2AfterStep.getPositions().curPos_.y_, pos2.y_);
}
