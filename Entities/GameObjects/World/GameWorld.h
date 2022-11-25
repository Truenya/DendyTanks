//
// Created by true on 2022-04-28.
//

#ifndef SSDL_GAMEWORLD_H
#define SSDL_GAMEWORLD_H

#include "vector"
#include "memory"
#include "../BaseGameObject.h"
#include "../../Utility.h"

struct StepReturn{
	enum {
		SUCCESS,
		MEET_WALL,
		MEET_PLAYER,
		MEET_PROJECTILE,
		OUT_OF_FIELD,
		UNDEFINED_BEHAVIOR
	} ret_;
	Position pos_;
};

class WorldGenerator;
struct MainProcessor;
typedef std::vector<std::vector<BaseGameObject>> MyGameWorldField;
struct GameWorld {
	void init(unsigned int x_dim, unsigned int y_dim);
	std::vector<Positions> update();
	Position size();
	BaseGameObject *player_{nullptr};
	StepReturn playerStep();
	bool addProjectile(Position);
	std::vector<Positions> allProjectilesStep();
	[[nodiscard]] BaseGameObject::Type typeAt(Position);
	void addPlayerPosChange(Position::Direction direction);
	void addProjectile(Position::Direction direction);
private:
	[[nodiscard]] BaseGameObject &at(Position);
	std::mutex changesMutex_;
	ManagedVector<Positions> playerPosChanges_;
	ManagedVector<Position> newProjectiles_;
	// TODO то что ниже объединить в структурку
	StepReturn playerStep(const Position &prev_pos, BaseGameObject::Type dst_type, Position &dst_pos);
	StepReturn projectileStep(const Position &prev_pos, BaseGameObject::Type dst_type, Position &dst_pos);
	void swapTypes(const Position &, const Position &);
    MyGameWorldField field_;
	ManagedVector<Position> projectiles_;
	friend WorldGenerator;
// TODO убираем
	friend BaseGameObject;
};


#endif //SSDL_GAMEWORLD_H
