//
// Created by true on 2022-04-30.
//

#include <iostream>
#include "CommandsProcessor.h"
#include "../Entities/Commands/MoveCommandExample.h"

CommandsProcessor::CommandsProcessor(MyGameWorld& world):
syncStreamErrors_(std::cerr),
world_(world) {
}

void CommandsProcessor::addCommand(BaseCommand *command) {
	std::unique_ptr<BaseCommand> com;
	com.reset(command);
	mutexCommands.lock();
	commands_.emplace_back(std::move(com));
	mutexCommands.unlock();
}

bool CommandsProcessor::processCommands() {
	if (commands_.empty())
		return false;
	std::vector <BaseCommandPtr> empty;
	mutexCommands.lock();
	std::swap(empty,commands_);
	mutexCommands.unlock();

	bool status = false;
	// Return status tells, need or not rerender screen
	for (const auto &command: empty) {
		status |= processMoveCommand(command);
	}
	return status;
}

bool CommandsProcessor::processMoveCommand(const BaseCommandPtr &command) {
	if (command->type == BaseCommand::Type::MoveCommand) {
		return processPlayerMove(command);
	}
	return false;
}

// For other player commands please create new methods
bool CommandsProcessor::processPlayerMove(const BaseCommandPtr &command) {
	if (command->obj->type == BaseGameObject::Type::Player) {
		auto positions = command->obj->getPositions();
		if (positions.curPos == getPlayer()->getPositions().curPos)
		{
			auto mv_command = reinterpret_cast<MoveCommandExample*>(command.get());
			auto delta = mv_command->dPos_;
			auto newPos = delta + positions.curPos;
			if ((newPos) && (world_.at(newPos).type != BaseGameObject::Type::Space))
				return false;
			if (command->execute().getType()) {
				syncStreamErrors_ << "Object is not initialized.\n";
				syncStreamErrors_.emit();
				return false;
			}
			changedPositions.emplace_back(positions.curPos,newPos);
			if(world_.at(newPos).type == BaseGameObject::Type::Player) {
				world_.player_ = &world_.at(newPos);
			}
			else {
				syncStreamErrors_ << "Trying to move non-player object.\n";
				syncStreamErrors_.emit();
			}

		}
	}
	return true;
}

const renderInfo CommandsProcessor::getChangedPositions() {
	renderInfo out;
	mutexCommands.lock();
	std::swap(out,changedPositions);
	mutexCommands.unlock();
	return out;
}

BaseGameObject *CommandsProcessor::getPlayer() const {
	return world_.player_;
}

const Position CommandsProcessor::worldSize() const {
	return world_.size();
}

const BaseGameObject &CommandsProcessor::at(const Position &pos) const {
	return world_.at(pos);
}
