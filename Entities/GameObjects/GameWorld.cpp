//
// Created by true on 2022-04-28.
//

#include "GameWorld.h"

#include <cassert>
#include <stdexcept>
GameWorld::GameWorld (unsigned int x_dim, unsigned int y_dim)
{
	if (x_dim > 10000 || y_dim > 10000)
		throw std::logic_error ("trying to construct too big world");
	field_.resize (x_dim);
	for (auto &row : field_) {
		row.resize (y_dim);
		for (auto &point : row) {
			point = GameObject::Type::UNDEFINED;
		}
	}
	projectiles_.init (100);
}

GameWorld::~GameWorld () = default;

Position GameWorld::size ()
{
	return {static_cast<int> (field_.size ()),
	        static_cast<int> (field_.front ().size ()),
	        0, Position::Direction::EQUAL};
}

GameObject::Type &GameWorld::at (Position pos)
{
	return field_[pos.x_][pos.y_];
}

StepReturn GameWorld::tankStep (const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos)
{
	if (!dst_pos)
		//		return {StepReturn::UNDEFINED_BEHAVIOR,prev_pos};
		return {StepReturn::UNDEFINED_BEHAVIOR};
	switch (dst_type)
	{
		case GameObject::Type::SPACE: {
			const auto TYPE = field_[prev_pos.x_][prev_pos.y_];
			auto tank = GameObject (prev_pos, TYPE);
			// Дадим объекту знать свои координаты
			tank.step ();
			assert (at (dst_pos) == GameObject::Type::SPACE);
			assert (at (prev_pos) == TYPE);
			// Скажем миру, что в мире объекты поменялись местами
			std::swap (at (prev_pos), at (dst_pos));
			assert (at (dst_pos) == TYPE);
			assert (at (prev_pos) == GameObject::Type::SPACE);
			return {StepReturn::SUCCESS};
			//			return {StepReturn::SUCCESS, dst_pos};
		}
		case GameObject::Type::PROJECTILE: {
			at (prev_pos) = GameObject::Type::SPACE;
			at (dst_pos) = GameObject::Type::SPACE;
			return {StepReturn::SUCCESS};
			//			return {StepReturn::SUCCESS, dst_pos};
		}
		case GameObject::Type::ENEMY:
			[[fallthrough]];
		case GameObject::Type::WALL: {
			//			return {StepReturn::MEET_WALL, prev_pos};
			return {StepReturn::MEET_WALL};
		}
			//		default: return {StepReturn::UNDEFINED_BEHAVIOR, prev_pos};
		default: return {StepReturn::UNDEFINED_BEHAVIOR};
	}
}

StepReturn GameWorld::projectileStep (const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos)
{
	const auto PREV_TYPE = typeAt (prev_pos);
	if (PREV_TYPE != GameObject::Type::PROJECTILE)
		// FIXME он сюда заходить не должен, если в мире там SPACE
		// скорее всего ситуация в том, что один выстрел взорвал другого
		projectiles_.remove (prev_pos);
	switch (dst_type)
	{
		case GameObject::Type::SPACE: {
			const auto TYPE = typeAt (prev_pos);
			assert (TYPE != GameObject::Type::PLAYER);
			if (typeAt (dst_pos) == GameObject::Type::PLAYER) throw std::runtime_error ("\n\nВы убиты.\n\n");
			dst_pos.reverseDirection ();
			std::swap (at (prev_pos), at (dst_pos));              // Скажем миру, что в мире объекты поменялись местами
			assert (typeAt (prev_pos) == GameObject::Type::SPACE);// Мы же не могли шагнуть из другой клетки
			return {StepReturn::SUCCESS};
		}
		case GameObject::Type::PROJECTILE: {
			if (dst_pos.direction_ == prev_pos.direction_)
				return {StepReturn::SUCCESS};
			at (prev_pos) = GameObject::Type::SPACE;
			at (dst_pos) = GameObject::Type::SPACE;
			return {StepReturn::MEET_PROJECTILE};
		}
		case GameObject::Type::WALL: {
			at (prev_pos) = GameObject::Type::SPACE;
			at (dst_pos) = GameObject::Type::SPACE;
			return {StepReturn::MEET_WALL};
		}
		case GameObject::Type::PLAYER: {
			at (prev_pos) = GameObject::Type::SPACE;
			at (dst_pos) = GameObject::Type::SPACE;
			return {StepReturn::MEET_PLAYER};
		}
		case GameObject::Type::ENEMY: {
			at (prev_pos) = GameObject::Type::SPACE;
			at (dst_pos) = GameObject::Type::SPACE;
			std::string destroyed;
			for (const auto &[k, tank] : tanks_) {
				if (tank.getPositions ().curPos_ == dst_pos) {
					destroyed = k;
					break;
				}
			}
			tanks_.erase (destroyed);
			return {StepReturn::MEET_ENEMY};
		}
		default:
			return {StepReturn::UNDEFINED_BEHAVIOR};
	}
}

//void GameWorld::swapTypes (const Position &pos, const Position &dst_pos)
//{
//	const auto first_type = at(pos).type_;
//	const auto second_type = at(dst_pos).type_;
//	std::swap(at(pos).type_,at(dst_pos).type_);
//}

StepReturn GameWorld::step (Positions pos)
{
	auto dst_pos = pos.curPos_;
	const auto PREV_POS = dst_pos;
	const auto PREV_TYPE = typeAt (PREV_POS);
	//	assert((PREV_TYPE == GameObject::Type::PLAYER)||(PREV_TYPE == GameObject::Type::ENEMY)); //FIXME
	dst_pos.stepInDirection ();
	// Check is new pos valid
	if (!dst_pos || !dst_pos.isValidByWorldSize (size ()))
		return {StepReturn::OUT_OF_FIELD};
	const auto DST_TYPE = typeAt (dst_pos);
	dst_pos.direction_ = PREV_POS.direction_;
	return tankStep (PREV_POS, DST_TYPE, dst_pos);
}

//StepReturn GameWorld::playerStep ()
//{
//	auto dst_pos = player_.getPositions().curPos_;
//	const auto PREV_POS = dst_pos;
//	const auto PREV_TYPE = typeAt(PREV_POS);
//	assert(PREV_TYPE == GameObject::Type::PLAYER);
//	dst_pos.stepInDirection();
//	// Check is new pos valid
//	if (!dst_pos || !dst_pos.isValidByWorldSize(size()))
//		return {StepReturn::OUT_OF_FIELD};
//	const auto DST_TYPE = typeAt(dst_pos);
//	dst_pos.direction_ = PREV_POS.direction_;
//	return tankStep (PREV_POS, DST_TYPE, dst_pos);
//}
#include <iostream>
#include <unordered_set>
std::vector<Positions> GameWorld::allProjectilesStep ()
{
	std::vector<Positions> output{};
	output.reserve (projectiles_.count () * 2);
	std::unordered_set<size_t> explosed;
	for (size_t i = 0; i < projectiles_.count (); ++i)
	{
		auto pos = projectiles_[i];
		const Position PREV_POS = pos;
		if (!pos.stepInDirection () || !pos.isValidByWorldSize (size ()) || !pos.isValid ())
		{
			explosed.insert (i);
			at (PREV_POS) = GameObject::Type::SPACE;
			continue;
		}
		auto s_r = projectileStep (PREV_POS, typeAt (pos), pos);
		switch (s_r.ret_) {
			case StepReturn::MEET_WALL:
				explosed.insert (i);
				[[fallthrough]];
			case StepReturn::SUCCESS:
				projectiles_[i].stepInDirection ();
				break;
			case StepReturn::UNDEFINED_BEHAVIOR:
				throw std::runtime_error ("UB. GameWorld::allProjectilesStep");
			case StepReturn::MEET_PLAYER:
				std::cerr << "\n\n----Вы убиты ----\n\n\t=(\n";
				tanks_.erase (my_uuid ());
				for (const auto &[k, _] : tanks_) {
					my_uuid () = k;
					break;
				}
				playerAlive = false;
				break;
			case StepReturn::MEET_PROJECTILE:
				[[fallthrough]];
			case StepReturn::MEET_ENEMY:
				if (tanks_.size () == 1)
					std::cout << "\n\n----Вы победили.----\n\n\t(-^-^-)\n";
				[[fallthrough]];
			case StepReturn::OUT_OF_FIELD:
				[[fallthrough]];
			default:
				explosed.insert (i);
				break;
		}
		output.emplace_back (Positions{PREV_POS, pos});
	}

	// TODO некрасиво, поправить
	// А вроде и норм..
	for (const auto EXPLOSE : explosed) {
		const Position EXPL = projectiles_[EXPLOSE];
		output.emplace_back (Positions{EXPL, EXPL});//,{},{}});
		projectiles_.remove (EXPLOSE);
	}
	return output;
}

bool GameWorld::addProjectile (Position pos)
{
	if (!pos)
		return false;

	if (typeAt (pos) == GameObject::Type::PLAYER) {
		std::cerr << "\n\n----Вы убиты ----\n\n\t=(\n";
		tanks_.erase (my_uuid ());
		for (const auto &[k, _] : tanks_) {
			my_uuid () = k;
			break;
		}
		playerAlive = false;
	}
	at (pos) = GameObject::Type::PROJECTILE;
	//assert(pos.direction_ != Position::Direction::EQUAL); FIXME
	projectiles_.add (pos);
	return true;
}

#include "Processors/Utils.h"
std::string GameWorld::addTank (const GameObject &tank)
{
	std::string uid = uuid::generate_uuid_v4 ();
	tanks_[uid] = tank;
	if (tank.type_ == GameObject::Type::PLAYER) my_uuid () = uid;
	return uid;
}

//std::vector<Positions> GameWorld::update ()
//{
//	// Изменения проходят на каждой итерации цикла (mainLoop).
//	// Накидывается в список для изменений в других потоках.
//	auto changed_positions = allProjectilesStep();
//	const auto LAST_MOVE_COMMAND = playerPosChanges_[playerPosChanges_.count() - 1];
//
//	const auto PREV_POS = player_->getPositions().curPos_;
//	assert(PREV_POS == LAST_MOVE_COMMAND.prevPos_);
//
//	auto ret = tankStep();
//	if (ret.ret_ == StepReturn::SUCCESS)
//	{
//		assert(PREV_POS == player_->getPositions().prevPos_);
//		changed_positions.emplace_back(Positions{PREV_POS, ret.pos_, player_->getPositions().dPos_, player_->getPositions().dstPos_});
//	}
//	return changed_positions;
//}
//
//void GameWorld::addPlayerPosChange (Position::Direction direction)
//{
//	auto pos = player_->getPositions().curPos_;
//	const auto PREV_POS = pos;
//	pos.direction_ = direction;
//	pos.stepInDirection();
//	if (pos && at(pos).type_ == GameObject::Type::SPACE)
//	{
//		const std::lock_guard<std::mutex> LOCK(changesMutex_);
//		playerPosChanges_.add(Positions{PREV_POS,pos, pos - PREV_POS,{}});
////		playerPosChanges_.count_++;
////		playerPosChanges_.c_.emplace_back(Positions{PREV_POS,pos, pos - PREV_POS,{}});
//	}
//}
//
//void GameWorld::addProjectile (Position::Direction direction)
//{
//	auto pos = player_->getPositions().curPos_;
//
//	pos.direction_ = direction;
//	pos.stepInDirection();
//	if (pos && at(pos).type_ != GameObject::Type::EQUAL)
//	{
//		const std::lock_guard<std::mutex> LOCK(changesMutex_);
//		newProjectiles_.add(pos);
////		newProjectiles_.count_++;
////		newProjectiles_.c_.emplace_back(pos);
//	}
//
//}

GameObject::Type GameWorld::typeAt (Position pos)
{
	if ((pos.x_ < 0) || (pos.y_ < 0))
		return GameObject::Type::UNDEFINED;
	if (pos.isValid () && pos.isValidByWorldSize ({static_cast<ssize_t> (field_.size ()), static_cast<ssize_t> (field_[0].size ()), 0, Position::Direction::TOP}))
		return at (pos);
	return GameObject::Type::UNDEFINED;
}
