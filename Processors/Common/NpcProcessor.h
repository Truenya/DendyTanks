//
// Created by true on 2022-05-22.
//

#ifndef SSDL_NPCPROCESSOR_H
#define SSDL_NPCPROCESSOR_H
#include "Entities/Commands/BaseCommand.h"
#include "Entities/GameObjects/GameWorld.h"
#include "Entities/Utility.h"

struct NpcStepData{
	ManagedVector <BaseCommand> NpcShoots; // BaseCommand
	ManagedVector <BaseCommand> NpcMooves; // BaseCommand
	explicit NpcStepData(size_t count_of_enemies)
	{
		NpcShoots.init(count_of_enemies);
		NpcMooves.init(count_of_enemies);
	}
};

// TODO прикрутить нейронки, пусть сами с собой играют учатся
class NpcProcessor
{
	GameWorld &world_;
	std::unordered_map <std::string, GameObject> &enemies_;
	bool canShoot(Position& pos, Position::Direction &direction);
	bool tryShootInDirection(const Position& pos, const Position::Direction &direction);
public:
	explicit NpcProcessor(GameWorld &world);
	~NpcProcessor() = default;
	NpcStepData step();
};


#endif //SSDL_NPCPROCESSOR_H
