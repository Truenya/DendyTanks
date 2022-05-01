//
// Created by true on 2022-04-30.
//

#ifndef SSDL_COMMANDSPROCESSOR_H
#define SSDL_COMMANDSPROCESSOR_H
#include <vector>
#include "../Entities/Commands/BaseCommand.h"
#include <memory>
#include <syncstream>
#include "../Entities/GameObjects/World/MyGameWorld.h"

typedef std::vector<std::pair<Position,Position>> renderInfo;
typedef std::unique_ptr<BaseCommand> BaseCommandPtr;

struct CommandsProcessor {
	CommandsProcessor(MyGameWorld &world);
	void addCommand(BaseCommand* command);
	bool processCommands();
	[[nodiscard]] const renderInfo getChangedPositions();

	BaseGameObject *getPlayer() const;
	[[nodiscard]]const Position worldSize() const;
	const BaseGameObject &at(const Position&) const;
private:
	renderInfo changedPositions;
	std::vector<BaseCommandPtr> commands_;
	std::osyncstream syncStreamErrors_;
	std::mutex mutexCommands;
	MyGameWorld &world_;
	bool processMoveCommand(const BaseCommandPtr &command);

	bool processPlayerMove(const BaseCommandPtr &command);
};


#endif //SSDL_COMMANDSPROCESSOR_H
