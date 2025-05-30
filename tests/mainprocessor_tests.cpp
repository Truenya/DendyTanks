#include <gtest/gtest.h>
#include "Entities/GameObjects/GameWorld.h"
#include "Processors/Common/MainProcessor.h"

class MainProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a small game world for testing
        world = std::make_unique<GameWorld>(10, 10);
        processor = std::make_unique<MainProcessor>(*world);
    }

    void TearDown() override {
        processor.reset();
        world.reset();
    }

    std::unique_ptr<GameWorld> world;
    std::unique_ptr<MainProcessor> processor;
};

// Test the constructor
TEST_F(MainProcessorTest, Constructor) {
    EXPECT_NE(processor, nullptr);

    // Check that the world size is correct
    Position size = processor->worldSize();
    EXPECT_EQ(size.x_, 10);
    EXPECT_EQ(size.y_, 10);
}

// Test adding and processing commands
TEST_F(MainProcessorTest, AddAndProcessCommands) {
    // Create a tank in the world
    Position pos(5, 5, 0, Position::Direction::TOP);
    GameObject player(pos, GameObject::Type::PLAYER);
    std::string uuid = world->addTank(player);

    // Create a move command for the tank
    BaseCommand moveCmd;
    moveCmd.type_ = BaseCommand::Type::MOVE_COMMAND;
    moveCmd.uid = uuid;

    Positions positions;
    positions.curPos_ = pos;
    positions.curPos_.direction_ = Position::Direction::RIGHT;  // Move right

    moveCmd.positions_ = positions;

    // Add the command to the processor
    processor->addCommand(moveCmd);

    // Process commands
    processor->processCommands();

    // Check that the player has moved
    EXPECT_NE(world->tanks_[uuid].getPositions().curPos_, pos);
}