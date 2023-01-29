//
// Created by true on 2022-05-22.
//

#include "NpcProcessor.h"

NpcProcessor::NpcProcessor (GameWorld &world) :
		world_(world),
		enemies_(world.tanks_)
{

}
#include <thread>
#include "MainProcessor.h"

#include <cassert>
#include <experimental/random>

// FIXME нельзя менять данные из мира здесь, только читать
NpcStepData NpcProcessor::step ()
{
	NpcStepData step_data(enemies_.size());
	for (auto &[k, _enemy]: enemies_)
	{
//		assert(_enemy.getPositions().curPos_.direction_ != Position::Direction::EQUAL);
		const auto prev_uid = GameWorld::my_uuid();
		if (world_.playerAlive && (k == GameWorld::my_uuid())) continue;
		if (not world_.playerAlive && (k == GameWorld::my_uuid())) {
			// Если мы общий противник - назначим другого противника и пойдем убивать его
			for (const auto &[l, _]: enemies_)
			{
				if (l != k){
					GameWorld::my_uuid() = l;
					break;
				}
			}
		}
		Position enemy = _enemy.getPositions().curPos_;
		Position::Direction direction;
		if (canShoot(enemy, direction)){ // direction передается по ссылке и изменяется
			// СТРЕЛЯЕМ
			assert(enemy.direction_ != Position::Direction::EQUAL);
			enemy.direction_ = direction;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) != GameObject::Type::UNDEFINED) {
				enemy.reverseDirection();
				enemy.stepInDirection ();
				enemy.reverseDirection();
				assert(enemy.direction_ != Position::Direction::EQUAL);
				step_data.NpcShoots.add ({BaseCommand::Type::SHOOT_COMMAND, {enemy, enemy}, k});
			}
		} else {
			auto directions = enemy.directionsTo(world_.tanks_[GameWorld::my_uuid()].getPositions().curPos_);
			const auto choice = std::experimental::randint(0, 1);
			enemy.direction_ = choice ? directions.first : directions.second;
			if (enemy.direction_ == Position::Direction::EQUAL)	enemy.direction_ = choice ? directions.second : directions.first;
			const Position PREV_POS = enemy;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) == GameObject::Type::SPACE && enemy.direction_!=Position::Direction::EQUAL)
			{
				assert(enemy.direction_ != Position::Direction::EQUAL);
				if(!std::experimental::randint(0,5)) {
					enemy.reverseDirection ();
					enemy.stepInDirection ();
					enemy.reverseDirection ();
					step_data.NpcShoots.add ({BaseCommand::Type::SHOOT_COMMAND, {enemy, enemy}, k});
				} else
					step_data.NpcMooves.add({BaseCommand::Type::MOVE_COMMAND,{PREV_POS, enemy}, k});
				_enemy.rotate (direction);
			}
			else{

				enemy.reverseDirection();
				enemy.stepInDirection ();
				enemy.reverseDirection();
//				assert(enemy.direction_ != Position::Direction::EQUAL); FIXME
				step_data.NpcShoots.add ({BaseCommand::Type::SHOOT_COMMAND, {enemy, enemy}, k});
				_enemy.rotate (direction);
			}
		}
		if (prev_uid == k) GameWorld::my_uuid() = k; // Вернем старого общего противника
	}
	return  step_data;
}

bool NpcProcessor::canShoot (Position &pos, Position::Direction &direction)
{
	Position::Directions player_directions = pos.directionsTo(world_.tanks_[GameWorld::my_uuid()].getPositions().curPos_);
	direction = player_directions.first;
	if (tryShootInDirection(pos,direction)) return true;
	direction = player_directions.second;
	if (tryShootInDirection(pos,direction)) return true;
	return false;
}

bool NpcProcessor::tryShootInDirection (const Position &pos, const Position::Direction &direction)
{
	if (direction == Position::Direction::EQUAL) return false;
	Position tmp = pos;
	tmp.direction_ = direction;
	while (true)
	{
		tmp.stepInDirection();
		if (world_.typeAt(tmp) == GameObject::Type::PLAYER)
			return true;
		if (world_.typeAt(tmp) != GameObject::Type::SPACE)
			break;
	}
	return false;
}
