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
#include "ParticlesSystem.h"

typedef std::vector<std::pair<Position,Position>> RenderMoveInfo;
typedef RenderMoveInfo RenderShootInfo;

struct MainProcessor {
#ifndef MAKE_LOG
	MainProcessor(GameWorld &world); // NOLINT(google-explicit-constructor)
#else
	MainProcessor(GameWorld &world,std::osyncstream &logs); // NOLINT(google-explicit-constructor)
#endif
	void addPlayerCommand(BaseCommand &&command);
	bool processCommands();
	bool processParticlesMoving();
	[[nodiscard]] RenderMoveInfo getChangedPositions();
	[[nodiscard]] RenderMoveInfo getShoots();
	BaseGameObject *getPlayer() const;
	[[nodiscard]]Position worldSize() const;
	void processingLoop();
	// Используется для заполнения мира, до начала поступления команд, соответственно не страшно.
	BaseGameObject::Type typeAt(const Position&) const;
	ManagedVector<Position> projectiles_;
private:
	RenderMoveInfo playerMoveChangedPositions_;
	RenderShootInfo projectilePositions_;
	std::vector<BaseCommand> playerMoveCommands_;
	std::vector<BaseCommand> playerShootCommands_;
#ifdef MAKE_LOG
	std::osyncstream &logsSynchroStream_;
#endif
	std::osyncstream syncStreamErrors_;
	std::mutex mutexCommands_;
	GameWorld &world_;

	void processObjectMove(const BaseCommand &command);

	bool processPlayerShoot(const BaseCommand &command);

	bool processPlayerMoveCommands(std::vector<BaseCommand> &commands);
	bool processPlayerShootCommands(std::vector<BaseCommand> &commands);
	ParticlesSystem particlesSystem_;
	static StepReturn projectileStepSecond (const Position &prev_pos, BaseGameObject::Type dst_type, const Position &dst_pos);
	std::vector<Positions> allProjectilesStepSecond ();
};
#endif //SSDL_MAINPROCESSOR_H
