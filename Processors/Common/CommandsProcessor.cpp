//
// Created by true on 2022-04-30.
//

#include <iostream>
#include "CommandsProcessor.h"
#include "../../Entities/Commands/MoveCommand.h"
CommandsProcessor::CommandsProcessor(MyGameWorld& world):
	syncStreamErrors_(std::cerr),
	world_(world)
{
	particlesSystem_.init();
	for (auto &item: particlesSystem_.projectilesByDistances_) {
		item.init();
	}
}

void CommandsProcessor::addPlayerCommand(BaseCommand *command) {
	std::unique_ptr<BaseCommand> com;
	com.reset(command);
	if (command->type_ == BaseCommand::Type::SHOOT_COMMAND)
	{
		mutexCommands_.lock();
		playerShootCommands_.emplace_back(std::move(com));
		mutexCommands_.unlock();
	}
	else
	{
		mutexCommands_.lock();
		playerMoveCommands_.emplace_back(std::move(com));
		mutexCommands_.unlock();
	}
}

bool CommandsProcessor::processCommands() {
	bool status = false;
	if (!playerMoveCommands_.empty()) {
		std::vector<BaseCommandPtr> empty;
		mutexCommands_.lock();
		std::swap(empty, playerMoveCommands_);
		mutexCommands_.unlock();

		// Return status tells, need or not rerender screen
		status |= processPlayerMoveCommands(empty);
	}else
	{
		status |= false;
	}
	if (!playerShootCommands_.empty()) {
		std::vector<BaseCommandPtr> empty;
		mutexCommands_.lock();
		std::swap(empty, playerShootCommands_);
		mutexCommands_.unlock();
		status |= processPlayerShootCommands(empty);
	}
	return status;
}

bool CommandsProcessor::processParticles() {
	auto particles = particlesSystem_.getCurrentPositions();
	for (const auto &particle: particles) {
		if (particle.first != particle.second) {
			world_.at(particle.first).type_ = BaseGameObject::Type::SPACE;
			world_.at(particle.second).type_ = BaseGameObject::Type::PROJECTILE;
		}
		projectilePositions_.emplace_back(particle);
	}
	if (!particles.empty())
		particlesSystem_.step();
	return true;
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
		status |= processObjectMove(command);
	}
	return status;
}


// For other commands please create new methods
bool CommandsProcessor::processObjectMove(const BaseCommandPtr &command) {
	auto positions = command->obj_->getPositions();
#ifndef RELEASE_BUILD
	// TODO добавить опцию в cmakelist и в зависимости от нее выставлять define
	// Управление потоком выполнения препятствуют предсказанию потока выполнения в процессоре
	// Поэтому в релизе подобных вещей быть не должно, по крайней мере в критичных
	// К скорости выполнения участках.
	if (command->obj_->type_ != BaseGameObject::Type::PLAYER) {
		syncStreamErrors_ << "Try to stepInDirection non player object\n";
		syncStreamErrors_.emit();
		return false;
	}
	if (command->obj_->getPositions().curPos_ != world_.player_->getPositions().curPos_) {
		syncStreamErrors_ << "Position of player in object is not matching to world->player position\n";
		syncStreamErrors_.emit();
		return false;
	}
#endif
	auto mv_command = reinterpret_cast<MoveCommand*>(command.get());
	auto delta = mv_command->dPos_;
	auto new_pos = positions.curPos_ + delta;
	// TODO legality of position could be processed on Position +/- operators
	if ((new_pos) && (world_.at(new_pos).type_ != BaseGameObject::Type::SPACE))
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
	playerMoveChangedPositions_.emplace_back(positions.curPos_, new_pos);
	// Let's the world know, where is player now
	if(world_.at(new_pos).type_ == BaseGameObject::Type::PLAYER) {
		world_.player_ = &world_.at(new_pos);
	}
	else {
		syncStreamErrors_ << "Trying to stepInDirection non-player object.\n";
		syncStreamErrors_.emit();
	}



	return true;
}
// If player press SPACE - lets shoot
bool CommandsProcessor::processPlayerShoot(const BaseCommandPtr &command) {
	// take position of player
	auto first_shoot_render_place = command->obj_->getPositions().curPos_;
	// take position in front of player
	first_shoot_render_place.stepInDirection();

	// let's find where is next wall...
	auto final_dst = first_shoot_render_place;
	if (first_shoot_render_place) {
		if (world_.at(first_shoot_render_place).type_ == BaseGameObject::Type::SPACE) {
			// if position is legal and on position is not placed walls
			while (final_dst && world_.at(final_dst).type_ != BaseGameObject::Type::WALL) {
				final_dst.stepInDirection();
			}
			// If reached illegal place - step back
			if (!final_dst)
				final_dst.stepInDirection(true);

			// Lets make big BOOM in front of tank in moment of shooting
			projectilePositions_.emplace_back(first_shoot_render_place, first_shoot_render_place);

			// distance to next wall
			auto distance = first_shoot_render_place.calcDistance(final_dst);
			// Let's remember our targets, and move to it step by step
			world_.at(first_shoot_render_place).setDestinationPoint(final_dst);
			// If we shot not point-blank - lets make projectile obj_, that will move till next wall
			if (world_.at(first_shoot_render_place).type_ == BaseGameObject::Type::SPACE) {
				world_.at(first_shoot_render_place).type_ = BaseGameObject::Type::PROJECTILE;
				world_.at(first_shoot_render_place).move({0,0,0,command->obj_->getPositions().curPos_.mDirection_});
			}
			// Insert this projectile to particle system, let it deside where is projectile in every step
			particlesSystem_.insertByDistance(distance, &world_.at(first_shoot_render_place));
			particlesSystem_.step();
			return true;
		}
		else{
			if (world_.at(first_shoot_render_place).type_ == BaseGameObject::Type::WALL
				||
			    world_.at(first_shoot_render_place).type_ == BaseGameObject::Type::PROJECTILE)
			{
				world_.at(first_shoot_render_place).type_=BaseGameObject::Type::SPACE;
				// Lets make big BOOM in front of tank in moment of shooting
				projectilePositions_.emplace_back(first_shoot_render_place, first_shoot_render_place);
				return true;
			}
		}
	}
	return false;
}


RenderMoveInfo CommandsProcessor::getChangedPositions() {
	RenderMoveInfo out;
	mutexCommands_.lock();
	std::swap(out, playerMoveChangedPositions_);
	mutexCommands_.unlock();
	return out;
}

BaseGameObject *CommandsProcessor::getPlayer() const {
	return world_.player_;
}

Position CommandsProcessor::worldSize() const {
	return world_.size();
}

const BaseGameObject &CommandsProcessor::at(const Position &pos) const {
	return world_.at(pos);
}

RenderShootInfo CommandsProcessor::getShoots()
{
	RenderShootInfo empty;
	mutexCommands_.lock();
	std::swap (empty, projectilePositions_);
	mutexCommands_.unlock();
	return empty;
}
