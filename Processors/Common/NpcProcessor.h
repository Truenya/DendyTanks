//
// Created by true on 2022-05-22.
//

#ifndef SSDL_NPCPROCESSOR_H
#define SSDL_NPCPROCESSOR_H
#include "Entities/GameObjects/World/GameWorld.h"
#include "Entities/Utility.h"

struct NpcStepData{
	ManagedVector <Positions> NpcShoots;
	ManagedVector <Positions> NpcMooves;
	NpcStepData(size_t count_of_enemies)
	{
		NpcShoots.init(count_of_enemies);
		NpcMooves.init(count_of_enemies);
	}
};

class NpcProcessor
{
	GameWorld &world_;
	ManagedVector <Position> enemies_;
	bool canShoot(Position& pos, Position::Direction &direction);
	bool tryShootInDirection(const Position& pos, const Position::Direction &direction);
public:
	NpcProcessor(GameWorld &world);
	~NpcProcessor() = default;
	NpcStepData step();
};


#endif //SSDL_NPCPROCESSOR_H
