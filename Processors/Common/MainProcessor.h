//
// Created by true on 2022-04-30.
//

#ifndef SSDL_MAINPROCESSOR_H
#define SSDL_MAINPROCESSOR_H
#include "../../Entities/Commands/BaseCommand.h"
#include <vector>
#include <memory>
#include <syncstream>
#include "../../Entities/GameObjects/World/GameWorld.h"
#include "NpcProcessor.h"

typedef std::vector<std::pair<Position,Position>> RenderMoveInfo;
typedef RenderMoveInfo RenderShootInfo;

struct MainProcessor {
#ifndef MAKE_LOG
	MainProcessor(GameWorld &world); // NOLINT(google-explicit-constructor)
#else
	MainProcessor(GameWorld &world,std::osyncstream &logs); // NOLINT(google-explicit-constructor)
#endif
	void addCommand(BaseCommand &&command);
	bool processCommands();
	bool processProjectilesMoving();
	bool processNpc();
	[[nodiscard]] RenderMoveInfo getPlayerChangedPositions();
	[[nodiscard]] RenderMoveInfo getNpcChangedPositions();
	[[nodiscard]] RenderMoveInfo getShoots();
//	GameObject getPlayer() const;
	[[nodiscard]] Position worldSize() const;
//	void processingLoop();
	// Используется для заполнения мира, до начала поступления команд, соответственно не страшно.
	GameObject::Type typeAt(const Position&) const;
	ManagedVector<Position> projectiles_;
private:
	RenderMoveInfo playerMoveChangedPositions_;
	RenderMoveInfo npcMoveChangedPositions_;
	RenderShootInfo projectilePositions_;
	GameWorld &world_;
	NpcProcessor npcProcessor_;
	std::vector<BaseCommand> playerMoveCommands_;
	std::vector<BaseCommand> npcMoveCommands_;
	std::vector<BaseCommand> shootCommands_;
#ifdef MAKE_LOG
	std::osyncstream &logsSynchroStream_;
#endif
	std::osyncstream syncStreamErrors_;
	std::mutex mutexCommands_;

	void processPlayerMove(const BaseCommand &command);

	bool processShoot(const BaseCommand &command);

	bool processPlayerMoveCommands(std::vector<BaseCommand> &commands);
	bool processShootCommands(std::vector<BaseCommand> &commands);
//	ParticlesSystem particlesSystem_;
//	static StepReturn projectileStepSecond (const Position &prev_pos, GameObject::Type dst_type, const Position &dst_pos);
//	std::vector<Positions> allProjectilesStepSecond ();
};
#endif //SSDL_MAINPROCESSOR_H
