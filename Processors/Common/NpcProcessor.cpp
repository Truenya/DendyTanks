//
// Created by true on 2022-05-22.
//

#include "NpcProcessor.h"

NpcProcessor::NpcProcessor (GameWorld &world) :
		world_(world),
		enemies_(world.enemies_)
{

}

NpcStepData NpcProcessor::step ()
{
	NpcStepData step_data(enemies_.count());
	for (size_t i = 0; i < enemies_.count(); ++i)
	{
		Position enemy = enemies_[i];
		Position::Direction direction;
		if (canShoot(enemy,direction)){
			// СТРЕЛЯЕМ
			enemy.direction_ = direction;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) != GameObject::Type::UNDEFINED)
			{
				// сходили - оставили выстрел, вернулись и развернулись  =В
				step_data.NpcShoots.add({enemy, enemy});//, {}, {}});
				enemy.reverseDirection();
				enemy.stepInDirection();
				enemy.reverseDirection();
			}
		}
		else
		{
			auto directions = enemy.directionsTo(world_.player_.getPositions().curPos_);
			enemy.direction_ = directions.first;
			const Position PREV_POS = enemy;
			enemy.stepInDirection();
			if (world_.typeAt(enemy) == GameObject::Type::SPACE)
			{
				step_data.NpcMooves.add({PREV_POS, enemy});//, {}, {}});
			}
			else{
				enemy.reverseDirection();
				enemy.stepInDirection();
				enemy.direction_ = directions.second;
				enemy.stepInDirection();
				if (world_.typeAt(enemy) == GameObject::Type::SPACE)
				{
					step_data.NpcMooves.add({PREV_POS, enemy});//, {}, {}});
				}
				else{
					enemy.reverseDirection();
					enemy.stepInDirection();
					enemy.reverseDirection();
				}
			}
		}
	}
	return  step_data;
}

bool NpcProcessor::canShoot (Position &pos,Position::Direction &direction)
{
	Position::Directions player_directions = pos.directionsTo(world_.player_.getPositions().curPos_);
	direction = player_directions.first;
	if (tryShootInDirection(pos,direction)) return true;
	direction = player_directions.second;
	if (tryShootInDirection(pos,direction)) return true;
	return false;
}

bool NpcProcessor::tryShootInDirection (const Position &pos, const Position::Direction &direction)
{
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
