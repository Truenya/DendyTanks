//
// Created by true on 2022-04-28.
//

#include "GameWorld.h"

#include <assert.h>
GameWorld::GameWorld(unsigned int x_dim, unsigned int y_dim)
{
	if(x_dim > 10000 || y_dim > 10000)
		throw std::logic_error ("trying to construct too big world");
	field_.resize(x_dim);
	int x = 0;
	for (auto &row: field_) {
		row.resize(y_dim);
		int y = 0;
		for (auto &point: row) {
			point.type_ = BaseGameObject::Type::UNDEFINED;
			++y;
		}
		++x;
	}
}

GameWorld::~GameWorld() = default;

Position GameWorld::size() {
	return {static_cast<int>(field_.size()),static_cast<int>(field_.front().size()),0,Position::Direction::UNDEFINED};
}

BaseGameObject &GameWorld::at(Position pos) {
 	return field_[pos.x_][pos.y_];
}

StepReturn GameWorld::playerStep (const Position &prev_pos, BaseGameObject::Type dst_type,const Position &dst_pos)
{
	switch (dst_type)
	{
		case BaseGameObject::Type::SPACE:
		{
			// Дадим объекту знать свои координаты
			at(prev_pos).step();
			// Скажем миру, что в мире объекты поменялись местами
			swapTypes(prev_pos, dst_pos);
			at(dst_pos).rotate(prev_pos.direction_);
			// И обновим указатель на игрока
			player_ = &at(dst_pos);
			return {StepReturn::SUCCESS, dst_pos};
		}
			break;
		case BaseGameObject::Type::PROJECTILE:
		{
			at(prev_pos).type_ = BaseGameObject::Type::SPACE;
			at(dst_pos).type_ = BaseGameObject::Type::SPACE;
			player_ = nullptr;
			return {StepReturn::SUCCESS, dst_pos};
		}
		case BaseGameObject::Type::WALL:
		{
			return {StepReturn::MEET_WALL, {}};
		}
		default: return {StepReturn::UNDEFINED_BEHAVIOR, {}};
	}
}

StepReturn GameWorld::projectileStep (const Position &prev_pos, BaseGameObject::Type dst_type,const  Position &dst_pos)
{
	switch (dst_type)
	{
		case BaseGameObject::Type::SPACE:
		{
			at(prev_pos).rotate(prev_pos.direction_);
			// Дадим объекту знать свои координаты
			at(prev_pos).step();
			// Скажем миру, что в мире объекты поменялись местами
			swapTypes(prev_pos,dst_pos);
			// И обновим указатель на игрока
			player_ = &at(dst_pos);
			return {StepReturn::SUCCESS, dst_pos};
		}
			break;
		case BaseGameObject::Type::PROJECTILE:
		{
			at(prev_pos).type_ = BaseGameObject::Type::SPACE;
			at(dst_pos).type_ = BaseGameObject::Type::SPACE;
			return {StepReturn::MEET_PROJECTILE, dst_pos};
		}
		case BaseGameObject::Type::WALL:
		{
			at(prev_pos).type_ = BaseGameObject::Type::SPACE;
			at(dst_pos).type_ = BaseGameObject::Type::SPACE;
			return {StepReturn::MEET_WALL, dst_pos};
		}
		case BaseGameObject::Type::PLAYER:
		{
			at(prev_pos).type_ = BaseGameObject::Type::SPACE;
			at(dst_pos).type_ = BaseGameObject::Type::SPACE;
			return {StepReturn::MEET_PLAYER, dst_pos};
		}
		default:
			return {StepReturn::UNDEFINED_BEHAVIOR,{}};
	}
}

void GameWorld::swapTypes (const Position &pos, const Position &dst_pos)
{
	std::swap(at(pos).type_,at(dst_pos).type_);
}

StepReturn GameWorld::playerStep ()
{
	// Check is new pos valid
	auto dst_pos = player_->getPositions().curPos_;
	const auto POS = dst_pos;
	assert(at(POS).type_ == BaseGameObject::Type::PLAYER);
	dst_pos.stepInDirection();
	if (!dst_pos)
		return {StepReturn::OUT_OF_FIELD,{}};
	const auto DST_TYPE = at(dst_pos).type_;

	dst_pos.direction_ = POS.direction_;
	return playerStep(POS,DST_TYPE,dst_pos);
}

std::vector<Positions> GameWorld::allProjectilesStep ()
{
	std::vector<Positions> output{};
	size_t count_of_explosed_projectiles = 0;
	for (size_t i = 0; i < projectiles_.count; ++i)
	{
		auto pos = projectiles_.c[i];
		const auto PREV_POS = pos;
		pos.stepInDirection();
		auto s_r = projectileStep(projectiles_.c[i], at(pos).type_,pos);
		if(s_r.ret_ == StepReturn::MEET_WALL || s_r.ret_ == StepReturn::MEET_PLAYER || s_r.ret_ == StepReturn::MEET_PROJECTILE)
		{
			count_of_explosed_projectiles++;
			continue;
		}
		const auto D_POS = pos - PREV_POS;
		projectiles_.c[i].stepInDirection();
		output.emplace_back(Positions{pos,PREV_POS,D_POS,{}});
	}
	projectiles_.count -= count_of_explosed_projectiles;
	return output;
}

bool GameWorld::addProjectile (Position pos)
{
	if (pos)
	{
		at(pos).type_ = BaseGameObject::Type::PROJECTILE;
		projectiles_.count++;
		projectiles_.c.emplace_back(pos);
		return true;
	}
	return false;
}

std::vector<Positions> GameWorld::update ()
{
	// Изменения проходят на каждой итерации цикла (mainLoop).
	// Накидывается в список для изменений в других потоках.
	auto changed_positions = allProjectilesStep();
	const auto LAST_MOVE_COMMAND = playerPosChanges_.c[playerPosChanges_.count];

	const auto PREV_POS = player_->getPositions().curPos_;
	assert(PREV_POS == LAST_MOVE_COMMAND.prevPos_);

	auto ret = playerStep();
	if (ret.ret_ == StepReturn::SUCCESS)
	{
		assert(PREV_POS == player_->getPositions().prevPos_);
		changed_positions.emplace_back(Positions{PREV_POS, ret.pos_, player_->getPositions().dPos_, player_->getPositions().dstPos_});
	}
	return changed_positions;
}

void GameWorld::addPlayerPosChange (Position::Direction direction)
{
	auto pos = player_->getPositions().curPos_;
	const auto PREV_POS = pos;
	pos.direction_ = direction;
	pos.stepInDirection();
	if (pos && at(pos).type_ == BaseGameObject::Type::SPACE)
	{

		const std::lock_guard<std::mutex> LOCK(changesMutex_);
		playerPosChanges_.count++;
		playerPosChanges_.c.emplace_back(Positions{PREV_POS,pos, pos - PREV_POS,{}});
	}
}

void GameWorld::addProjectile (Position::Direction direction)
{
	auto pos = player_->getPositions().curPos_;

	pos.direction_ = direction;
	pos.stepInDirection();
	if (pos && at(pos).type_ != BaseGameObject::Type::UNDEFINED)
	{
		const std::lock_guard<std::mutex> LOCK(changesMutex_);
		newProjectiles_.count++;
		newProjectiles_.c.emplace_back(pos);
	}

}
