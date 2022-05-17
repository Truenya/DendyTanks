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
	projectiles_.init(100);
}

GameWorld::~GameWorld() = default;

Position GameWorld::size() {
	return {static_cast<int>(field_.size()),static_cast<int>(field_.front().size()),0,Position::Direction::UNDEFINED};
}

BaseGameObject &GameWorld::at(Position pos) {
 	return field_[pos.x_][pos.y_];
}

StepReturn GameWorld::playerStep (const Position &prev_pos, BaseGameObject::Type dst_type, Position &dst_pos)
{
	if (!dst_pos)
		return {StepReturn::UNDEFINED_BEHAVIOR,prev_pos};
	switch (dst_type)
	{
		case BaseGameObject::Type::SPACE:
		{
			// Дадим объекту знать свои координаты
			at(prev_pos).step();
			at(dst_pos).rotate(prev_pos.direction_);
			dst_pos.reverseDirection();
			at(dst_pos).step();
			assert(at(dst_pos).type_ == BaseGameObject::Type::SPACE);
			assert(at(prev_pos).type_ == BaseGameObject::Type::PLAYER);
			// Скажем миру, что в мире объекты поменялись местами
			std::swap(at(prev_pos), at(dst_pos));
			assert(at(dst_pos).type_ == BaseGameObject::Type::PLAYER);
			assert(at(prev_pos).type_ == BaseGameObject::Type::SPACE);
			at(dst_pos).rotate(prev_pos.direction_);
			assert(at(dst_pos).type_ == BaseGameObject::Type::PLAYER);
			// И обновим указатель на игрока
			player_ = &at(dst_pos);
			assert(at(dst_pos).type_ == BaseGameObject::Type::PLAYER);
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

StepReturn GameWorld::projectileStep (const Position &prev_pos, BaseGameObject::Type dst_type, Position &dst_pos)
{
	const auto PREV_TYPE = typeAt(prev_pos);
	assert(PREV_TYPE == BaseGameObject::Type::PROJECTILE);
	switch (dst_type)
	{
		case BaseGameObject::Type::SPACE:
		{
			at(prev_pos).rotate(prev_pos.direction_);
			const auto TYPE = typeAt(prev_pos);
			assert(TYPE != BaseGameObject::Type::PLAYER);
			assert(TYPE != BaseGameObject::Type::SPACE);
			assert(typeAt(dst_pos) != BaseGameObject::Type::PLAYER);
			// Дадим объекту знать свои координаты
			at(prev_pos).step();
			at(dst_pos).rotate(prev_pos.direction_);
			dst_pos.reverseDirection();
			at(dst_pos).step();
			std::swap(at(prev_pos), at(dst_pos));
			// Скажем миру, что в мире объекты поменялись местами
			assert(typeAt(dst_pos) == BaseGameObject::Type::PROJECTILE);
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
	const auto first_type = at(pos).type_;
	const auto second_type = at(dst_pos).type_;
	std::swap(at(pos).type_,at(dst_pos).type_);
}

StepReturn GameWorld::playerStep ()
{
	// Check is new pos valid
	auto dst_pos = player_->getPositions().curPos_;
	const auto POS = dst_pos;
	const auto PREV_TYPE = typeAt(POS);
	assert(PREV_TYPE == BaseGameObject::Type::PLAYER);
	dst_pos.stepInDirection();
	if (!dst_pos || !dst_pos.isValidByWorldSize(size()))
		return {StepReturn::OUT_OF_FIELD,{}};
	const auto DST_TYPE = typeAt(dst_pos);

	dst_pos.direction_ = POS.direction_;
	return playerStep(POS,DST_TYPE,dst_pos);
}
#include <unordered_set>
std::vector<Positions> GameWorld::allProjectilesStep ()
{
	std::vector<Positions> output{};
	std::unordered_set<size_t> explosed;
	for (size_t i = 0; i < projectiles_.count(); ++i)
	{
		auto pos = projectiles_[i];
		const Position PREV_POS = pos;
		if (pos.stepInDirection() && pos.isValidByWorldSize(size()))
		{
			const auto D_POS = pos - PREV_POS;
			auto s_r = projectileStep(PREV_POS, typeAt(pos), pos);
			if (s_r.ret_ == StepReturn::MEET_WALL       || s_r.ret_ == StepReturn::MEET_PLAYER
	         || s_r.ret_ == StepReturn::MEET_PROJECTILE || s_r.ret_ == StepReturn::OUT_OF_FIELD)
				explosed.insert(i);
			else
				projectiles_[i].stepInDirection();
			output.emplace_back(Positions{PREV_POS, pos, D_POS, {}});
		}
		else{
			explosed.insert(i);
			at(PREV_POS).type_ = BaseGameObject::Type::SPACE;
		}
	}
	for (const auto EXPLOSE: explosed)
	{
		projectiles_.remove(EXPLOSE);
	}
	return output;
}

bool GameWorld::addProjectile (Position pos)
{
	if (pos)
	{
		assert(at(pos).type_ != BaseGameObject::Type::PLAYER);
		at(pos).type_ = BaseGameObject::Type::PROJECTILE;
		projectiles_.add(pos);
//		if (projectiles_.c_.size() < (projectiles_.count_ + 1))
//			projectiles_.c_.resize(projectiles_.count_+1);
//		projectiles_.c_[projectiles_.count_] = pos;
//		projectiles_.count_++;
		return true;
	}
	return false;
}

std::vector<Positions> GameWorld::update ()
{
	// Изменения проходят на каждой итерации цикла (mainLoop).
	// Накидывается в список для изменений в других потоках.
	auto changed_positions = allProjectilesStep();
	const auto LAST_MOVE_COMMAND = playerPosChanges_[playerPosChanges_.count() - 1];

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
		playerPosChanges_.add(Positions{PREV_POS,pos, pos - PREV_POS,{}});
//		playerPosChanges_.count_++;
//		playerPosChanges_.c_.emplace_back(Positions{PREV_POS,pos, pos - PREV_POS,{}});
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
		newProjectiles_.add(pos);
//		newProjectiles_.count_++;
//		newProjectiles_.c_.emplace_back(pos);
	}

}

const BaseGameObject::Type GameWorld::typeAt (Position pos)
{
	assert((pos.x_ >= 0) && (pos.y_ >= 0));
	return at(pos).type_;
}
