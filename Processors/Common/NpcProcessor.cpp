//
// Created by true on 2022-05-22.
//

#include "NpcProcessor.h"

NpcProcessor::NpcProcessor (GameWorld &world) :
		world_(world),
		enemies_(world.tanks_)
{

}
#include <experimental/random>

NpcStepData NpcProcessor::step ()
{
	NpcStepData step_data(enemies_.size());
	for (auto &[k, _enemy]: enemies_)
	{
		if (k == GameWorld::my_uuid()) continue;
		Position enemy = _enemy.getPositions().curPos_;
		Position::Direction direction;
		if (canShoot(enemy, direction)){ // direction передается по ссылке и изменяется
			// СТРЕЛЯЕМ
			enemy.direction_ = direction;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) != GameObject::Type::UNDEFINED)
				step_data.NpcShoots.add({BaseCommand::Type::SHOOT_COMMAND, {enemy, enemy}, k});
		} else {
			auto directions = enemy.directionsTo(world_.player_.getPositions().curPos_);
			enemy.direction_ = std::experimental::randint(0, 1) ? directions.first : directions.second ;
			const Position PREV_POS = enemy;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) == GameObject::Type::SPACE)
			{
				step_data.NpcMooves.add({BaseCommand::Type::MOVE_COMMAND,{PREV_POS, enemy}, k});//, {}, {}});
			}
			else{
				step_data.NpcShoots.add ({BaseCommand::Type::SHOOT_COMMAND, {enemy, enemy}, k});
			}
		}
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
