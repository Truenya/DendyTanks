//
// Created by true on 2022-04-30.
//

#include <iostream>
#include "CommandsProcessor.h"
#include "../Entities/Commands/MoveCommand.h"

CommandsProcessor::CommandsProcessor(MyGameWorld& world):
	syncStreamErrors_(std::cerr),
	world_(world)
{}

void CommandsProcessor::addPlayerCommand(BaseCommand *command) {
	std::unique_ptr<BaseCommand> com;
	com.reset(command);
	mutexCommands.lock();
	playerMoveCommands_.emplace_back(std::move(com));
	mutexCommands.unlock();
}

bool CommandsProcessor::processCommands() {
	bool status = false;
	if (!playerMoveCommands_.empty()) {
		std::vector<BaseCommandPtr> empty;
		mutexCommands.lock();
		std::swap(empty, playerMoveCommands_);
		mutexCommands.unlock();

		// Return status tells, need or not rerender screen
		status |= processPlayerMoveCommands(empty);
	}else
	{
		status |= false;
	}
	if (!playerMoveCommands_.empty()) {
		std::vector<BaseCommandPtr> empty;
		mutexCommands.lock();
		std::swap(empty, playerShootCommands_);
		mutexCommands.unlock();
		status |= processPlayerShootCommands(empty);
	}
	return status;
}

bool CommandsProcessor::processPlayerShootCommands(std::vector<BaseCommandPtr> &empty) {
	bool status = false;
	for (const auto &command: empty) {
		status |= processPlayerShoot(command);
	}
	return status;
}

bool CommandsProcessor::processPlayerMoveCommands(std::vector<BaseCommandPtr> &empty) {
	bool status = false;
	for (const auto &command: empty) {
		status |= processPlayerMove(command);
	}
	return status;
}


// For other player commands please create new methods
bool CommandsProcessor::processPlayerMove(const BaseCommandPtr &command) {
	auto positions = command->obj->getPositions();

	// TODO добавить опцию в cmakelist и в зависимости от нее выставлять define
	// Управление потоком выполнения препятствуют предсказанию потока выполнения в процессоре
	// Поэтому в релизе подобных вещей быть не должно, по крайней мере в критичных
	// К скорости выполнения участках.
#ifndef RELEASE_BUILD
	if (command->obj->type != BaseGameObject::Type::Player) {
		syncStreamErrors_ << "Try to stepInDirection non player object\n";
		syncStreamErrors_.emit();
		return false;
	}
	if (command->obj->getPositions().curPos != world_.player_->getPositions().curPos) {
		syncStreamErrors_ << "Position of player in object is not matching to world->player position\n";
		syncStreamErrors_.emit();
		return false;
	}
#endif
	auto mv_command = reinterpret_cast<MoveCommand*>(command.get());
	auto delta = mv_command->dPos_;
	auto newPos = positions.curPos + delta;
	if ((newPos) && (world_.at(newPos).type != BaseGameObject::Type::Space))
		return false;
	auto type = command->execute().getType();
	if (type != SdlErrorCodeExample::NO_ERRORS) {
		if (type == SdlErrorCodeExample::NOT_INITIALIZED_OBJECT)
			syncStreamErrors_ << "Object is not initialized.\n";
		else if (type == SdlErrorCodeExample::CANNOT_MOVE)
			syncStreamErrors_ << "Move outside of field or position is incorrect.\n";
		syncStreamErrors_.emit();
		return false;
	}
	playerMoveChangedPositions.emplace_back(positions.curPos, newPos);
	if(world_.at(newPos).type == BaseGameObject::Type::Player) {
		world_.player_ = &world_.at(newPos);
	}
	else {
		syncStreamErrors_ << "Trying to stepInDirection non-player object.\n";
		syncStreamErrors_.emit();
	}



	return true;
}

bool CommandsProcessor::processPlayerShoot(const BaseCommandPtr &command) {
	auto dst = command->obj->getPositions().curPos;
	dst.stepInDirection();
	auto final_dst = dst;
	if (dst && world_.at(dst).type == BaseGameObject::Type::Space)
	{
		while(!(final_dst && world_.at(final_dst).type == BaseGameObject::Type::Wall))
		{
			final_dst.stepInDirection();
		}
		if (!final_dst)
			final_dst.stepInDirection(true);
		playerShootPositions.emplace_back(dst, final_dst);
		return true;
	}
	return false;
}

const renderMoveInfo CommandsProcessor::getChangedPositions() {
	renderMoveInfo out;
	mutexCommands.lock();
	std::swap(out, playerMoveChangedPositions);
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

const renderShootInfo getShoots()
{
	renderShootInfo empty;
	mutexCommands.lock();
	std::swap (empty,playerShootPositions);
	mutexCommands.unlock();
	return empty;
}