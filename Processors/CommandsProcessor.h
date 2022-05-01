//
// Created by true on 2022-04-30.
//

#ifndef SSDL_COMMANDSPROCESSOR_H
#define SSDL_COMMANDSPROCESSOR_H
#include "../Entities/Commands/BaseCommand.h"
#include <vector>
#include <memory>
#include <syncstream>
#include "../Entities/GameObjects/World/MyGameWorld.h"

typedef std::vector<std::pair<Position,Position>> renderMoveInfo;
typedef renderMoveInfo renderShootInfo;
typedef std::unique_ptr<BaseCommand> BaseCommandPtr;

struct CommandsProcessor {
	CommandsProcessor(MyGameWorld &world);
	void addPlayerCommand(BaseCommand* command);
	bool processCommands();
	[[nodiscard]] const renderMoveInfo getChangedPositions();

	BaseGameObject *getPlayer() const;
	[[nodiscard]]const Position worldSize() const;
	const BaseGameObject &at(const Position&) const;
private:
	renderMoveInfo playerMoveChangedPositions;
	renderShootInfo playerShootPositions;
	std::vector<BaseCommandPtr> playerMoveCommands_;
	std::vector<BaseCommandPtr> playerShootCommands_;
	std::osyncstream syncStreamErrors_;
	std::mutex mutexCommands;
	MyGameWorld &world_;

	bool processPlayerMove(const BaseCommandPtr &command);

	bool processPlayerShoot(const BaseCommandPtr &command);

	bool processPlayerMoveCommands(std::vector<BaseCommandPtr> &empty);
	bool processPlayerShootCommands(std::vector<BaseCommandPtr> &empty);
};


#endif //SSDL_COMMANDSPROCESSOR_H
