//
// Created by true on 2022-04-30.
//

#ifndef SSDL_COMMANDSPROCESSOR_H
#define SSDL_COMMANDSPROCESSOR_H
#include "../../Entities/Commands/BaseCommand.h"
#include <vector>
#include <memory>
#include <syncstream>
#include "../../Entities/GameObjects/World/MyGameWorld.h"
#include "ParticlesSystem.h"


typedef std::vector<std::pair<Position,Position>> RenderMoveInfo;
typedef RenderMoveInfo RenderShootInfo;
typedef std::unique_ptr<BaseCommand> BaseCommandPtr;

struct CommandsProcessor {
	CommandsProcessor(MyGameWorld &world);
	void addPlayerCommand(BaseCommand* command);
	bool processCommands();
	bool processParticles();
	[[nodiscard]] RenderMoveInfo getChangedPositions();
	[[nodiscard]] RenderMoveInfo getShoots();

	BaseGameObject *getPlayer() const;
	[[nodiscard]]Position worldSize() const;
	const BaseGameObject &at(const Position&) const;
private:
	RenderMoveInfo playerMoveChangedPositions_;
	RenderShootInfo projectilePositions_;
	std::vector<BaseCommandPtr> playerMoveCommands_;
	std::vector<BaseCommandPtr> playerShootCommands_;
	std::osyncstream syncStreamErrors_;
	std::mutex mutexCommands_;
	MyGameWorld &world_;

	bool processObjectMove(const BaseCommandPtr &command);

	bool processPlayerShoot(const BaseCommandPtr &command);

	bool processPlayerMoveCommands(std::vector<BaseCommandPtr> &empty);
	bool processPlayerShootCommands(std::vector<BaseCommandPtr> &empty);
	ParticlesSystem particlesSystem_;
};


#endif //SSDL_COMMANDSPROCESSOR_H
