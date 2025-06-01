
#include <gtest/gtest.h>
#include "GameWorld.h"
#include "Position.h"

class GameWorldTests : public ::testing::Test {
protected:
    GameWorld *world;

public:
    void SetUp() override {
        world = new GameWorld(20, 15);
    }

    void TearDown() override {
        delete world;
    }
};

// Test constructor and size
TEST_F(GameWorldTests, ConstructorAndSize) {
    EXPECT_EQ(world->size().x_, 20);
    EXPECT_EQ(world->size().y_, 15);
}

// Test construction with invalid dimensions
TEST_F(GameWorldTests, ConstructorWithInvalidDimensions) {
    try {
        GameWorld *world = new GameWorld(10001, 1);
        delete world;
        FAIL() << "Expected exception was not thrown";
    } catch (const std::exception &e) {
        EXPECT_STREQ("trying to construct too big world", e.what());
    }
}

// Test typeAt method
TEST_F(GameWorldTests, TypeAt) {
    // Check that all positions are initially SPACE-like
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 10; y++) {
            Position pos(x, y, 0, Position::Direction::TOP);
            GameObject::Type type = world->typeAt(pos);
            EXPECT_TRUE(type == GameObject::Type::SPACE || type == static_cast<GameObject::Type>(0x00000000));
        }
    }

    // Test with invalid position
    Position invalidPos(-1, -1, 0, Position::Direction::TOP);
    GameObject::Type inv_type = world->typeAt(invalidPos);
    EXPECT_TRUE(inv_type == GameObject::Type::SPACE || inv_type == static_cast<GameObject::Type>(0x00000000));
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
    std::shared_ptr<Projectile> projectile = std::make_shared<Projectile>(pos);

    world->addProjectile(projectile);
    EXPECT_EQ(world->getProjectiles().size(), 1);
}

// Test step function
TEST_F(GameWorldTests, Step) {
    Position pos(5, 5, 0, Position::Direction::TOP);
    GameObject tank(pos, GameObject::Type::PLAYER);

    std::string uuid = world->addTank(tank);
    EXPECT_EQ(world->step(), true);
}

// Test getting world size
TEST_F(GameWorldTests, WorldSize) {
    Position pos(5, 5, 0, Position::Direction::TOP);
    GameObject tank(pos, GameObject::Type::PLAYER);

    std::string uuid = world->addTank(tank);
    auto size = world->size();
    EXPECT_EQ(size.x_, 20);
    EXPECT_EQ(size.y_, 15);
}

// Test typeAt method with different types
TEST_F(GameWorldTests, TypeAtWithDifferentTypes) {
    // Add a projectile
    Position projectilePos(4, 4, 0, Position::Direction::TOP);
    world->addProjectile(projectilePos);

    // Check that the position has the correct type
    GameObject::Type proj_type = world->typeAt(projectilePos);
    EXPECT_TRUE(proj_type == GameObject::Type::PROJECTILE || proj_type == static_cast<GameObject::Type>(0x03000000));

    // Check a position that should be SPACE-like
    Position emptyPos(7, 7, 0, Position::Direction::TOP);
    GameObject::Type empty_type = world->typeAt(emptyPos);
    EXPECT_TRUE(empty_type == GameObject::Type::SPACE || empty_type == static_cast<GameObject::Type>(0x00000000));

    // Check an invalid position
    Position invalidPos(-1, -1, 0, Position::Direction::TOP);
    GameObject::Type inv_type = world->typeAt(invalidPos);
    EXPECT_TRUE(inv_type == GameObject::Type::SPACE || inv_type == static_cast<GameObject::Type>(0x00000000));
}

// Test multiple steps with collision
TEST_F(GameWorldTests, MultipleStepsWithCollision) {
    // Add tank 1 at position (5,5)
    Position pos1(5, 5, 0, Position::Direction::TOP);
    GameObject tank1(pos1, GameObject::Type::PLAYER);

    // Add tank 2 at position (6,5)
    Position pos2(6, 5, 0, Position::Direction::TOP);
    GameObject tank2(pos2, GameObject::Type::ENEMY);

    std::string uuid1 = world->addTank(tank1);
    std::string uuid2 = world->addTank(tank2);

    // Move tank1 towards tank2
    for (int i = 0; i < 5; i++) {
        EXPECT_TRUE(world->step());
    }
}

