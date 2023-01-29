//
// Created by true on 2022-04-30.
//

#ifndef SSDL_MAINPROCESSOR_H
#define SSDL_MAINPROCESSOR_H
#include "../../Entities/Commands/BaseCommand.h"
#include "Entities/GameObjects/GameWorld.h"
#include "NpcProcessor.h"
#include <memory>
#include <syncstream>
#include <vector>

typedef std::vector<std::pair<Position,Position>> RenderMoveInfo;
typedef RenderMoveInfo RenderShootInfo;

struct MainProcessor {
#ifndef MAKE_LOG
	MainProcessor(GameWorld &world); // NOLINT(google-explicit-constructor)
#else
	MainProcessor(GameWorld &world,std::osyncstream &logs); // NOLINT(google-explicit-constructor)
#endif
	void addCommand(BaseCommand command);
	void processCommands();
	bool processProjectilesMoving();
	void processingNpcLoop (const atomic<bool> &working);
	bool noTankAtPos (const Position pos);
	[[nodiscard]] RenderMoveInfo getPlayerChangedPositions();
	[[nodiscard]] RenderMoveInfo getNpcChangedPositions();
	[[nodiscard]] RenderMoveInfo getShoots();
	[[nodiscard]] Position worldSize() const;
	// Используется для заполнения мира, до начала поступления команд, соответственно не страшно.
	GameObject::Type typeAt(const Position&) const;
	ManagedVector<Position> projectiles_;
private:
	RenderMoveInfo playerChangedPositions_;
	RenderMoveInfo npcChangedPositions_;
	RenderShootInfo projectilePositions_;
	GameWorld &world_;
	NpcProcessor npcProcessor_;
	std::vector<BaseCommand> moveCommands_;
	std::vector<BaseCommand> shootCommands_;
#ifdef MAKE_LOG
	std::osyncstream &logsSynchroStream_;
#endif
	std::osyncstream syncStreamErrors_;
	std::mutex mutexMoves_;
	std::mutex mutexShoots_;

	void processTankMove (const BaseCommand &command);
	bool processShoot(const BaseCommand &command);
	bool processMoveCommands (std::vector<BaseCommand> &commands);
	bool processShootCommands(std::vector<BaseCommand> &commands);
	void prepareCommands (std::vector<BaseCommand> &empty);
	void prepareShoots (std::vector<BaseCommand> &empty);
};
#endif //SSDL_MAINPROCESSOR_H
