//
// Created by true on 2022-04-30.
//


#include <iostream>
#include "MainProcessor.h"

#ifndef MAKE_LOG
MainProcessor::MainProcessor(GameWorld& world):
	syncStreamErrors_(std::cerr),
	world_(world)
{
	particlesSystem_.init();
	for (auto &item: particlesSystem_.projectilesByDistances_) {
		item.init();
	}
}
#else
MainProcessor::MainProcessor(GameWorld& world,std::osyncstream &logs):
		syncStreamErrors_(std::cerr),
		logsSynchroStream_(logs),
		world_(world)
{
	particlesSystem_.init();
	for (auto &item: particlesSystem_.projectilesByDistances_) {
		item.init();
	}
}
#endif


void MainProcessor::addPlayerCommand(BaseCommand &&command) {
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	if (command.type_ == BaseCommand::Type::SHOOT_COMMAND)
	{
		playerShootCommands_.emplace_back(std::move(command));
	}
	else
	{
		playerMoveCommands_.emplace_back(std::move(command));
	}
}

bool MainProcessor::processCommands() {
	bool status = false;
	if (!playerMoveCommands_.empty()) {
		std::vector<BaseCommand> empty;
		{
			const std::lock_guard<std::mutex> LOCK(mutexCommands_);
			std::swap(empty, playerMoveCommands_);
		}
		// Return status tells, need or not rerender screen
		status |= processPlayerMoveCommands(empty);
	}else
	{
		status |= false;
	}
	if (!playerShootCommands_.empty()) {
		std::vector<BaseCommand> empty;
		{
			const std::lock_guard<std::mutex> LOCK(mutexCommands_);
			std::swap(empty, playerShootCommands_);
		}
		status |= processPlayerShootCommands(empty);
	}
	return status;
}

bool MainProcessor::processParticlesMoving() {
	auto positions = world_.allProjectilesStep();
	if (!positions.empty())
	{
		for (const auto &position: positions)
		{
			projectilePositions_.emplace_back(position.prevPos_,position.curPos_);
		}

	}
	return true;
}

bool MainProcessor::processPlayerShootCommands(std::vector<BaseCommand> &commands) {
	bool status = false;
	for (const auto &command: commands) {
		status |= processPlayerShoot(command);
	}
	return status;
}

bool MainProcessor::processPlayerMoveCommands(std::vector<BaseCommand> &commands) {
	bool status = false;
	for (const auto &command: commands) {
		processObjectMove(command);
	}
	return status;
}


// For other commands please create new methods
void MainProcessor::processObjectMove(const BaseCommand &command) {
	world_.player_->rotate(command.position_.direction_);
	auto positions = command.obj_->getPositions();
	auto s_r = world_.playerStep();
#ifdef DEBUG
	// TODO добавить опцию в cmakelist и в зависимости от нее выставлять define
	// Управление потоком выполнения препятствуют предсказанию потока выполнения в процессоре
	// Поэтому в релизе подобных вещей быть не должно, по крайней мере в критичных
	// К скорости выполнения участках.
	if (command.obj_->type_ != BaseGameObject::Type::PLAYER) {
		syncStreamErrors_ << "Try to stepInDirection non player object\n";
		syncStreamErrors_.emit();
		return false;
	}
	if (command.obj_->getPositions().curPos_ != world_.player_->getPositions().curPos_) {
		syncStreamErrors_ << "Position of player in object is not matching to world->player position\n";
		syncStreamErrors_.emit();
		return false;
	}
#endif
	if (s_r.ret_ == StepReturn::SUCCESS)
		playerMoveChangedPositions_.emplace_back(positions.curPos_, s_r.pos_);
}
// If player press SPACE - lets shoot
bool MainProcessor::processPlayerShoot(const BaseCommand &command) {
	// take position of player
	auto first_shoot_render_place = command.obj_->getPositions().curPos_;
	first_shoot_render_place.stepInDirection();
	if (world_.addProjectile(first_shoot_render_place))
	{
		projectilePositions_.emplace_back(std::pair{
			command.obj_->getPositions().curPos_,command.obj_->getPositions().curPos_});
		return true;
	}
	return false;
}


RenderMoveInfo MainProcessor::getChangedPositions() {
	RenderMoveInfo out;
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	std::swap(out, playerMoveChangedPositions_);
	return out;
}

BaseGameObject *MainProcessor::getPlayer() const {
	return world_.player_;
}

Position MainProcessor::worldSize() const {
	return world_.size();
}

const BaseGameObject &MainProcessor::at(const Position &pos) const {
	return world_.at(pos);
}

RenderShootInfo MainProcessor::getShoots()
{
	RenderShootInfo empty;
	const std::lock_guard<std::mutex> LOCK(mutexCommands_);
	std::swap (empty, projectilePositions_);
	return empty;
}
