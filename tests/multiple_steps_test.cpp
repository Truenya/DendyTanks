#include <gtest/gtest.h>
#include "Entities/GameObjects/GameWorld.h"

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
