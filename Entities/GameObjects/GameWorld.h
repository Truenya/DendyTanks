//
// Created by true on 2022-04-28.
//

#ifndef SSDL_GAMEWORLD_H
#define SSDL_GAMEWORLD_H

#include "Entities/Utility.h"
#include "GameObject.h"
#include <string>
#include <unordered_map>
#include <vector>

struct StepReturn {
	enum
	{
		SUCCESS,
		MEET_WALL,
		MEET_PLAYER,
		MEET_ENEMY,
		MEET_PROJECTILE,
		OUT_OF_FIELD,
		UNDEFINED_BEHAVIOR
	} ret_{UNDEFINED_BEHAVIOR};
	//	Position pos_;
};

class WorldGenerator;
struct MainProcessor;
typedef std::vector<std::vector<GameObject::Type>> MyGameWorldField;
struct GameWorld {
	GameWorld (unsigned int x_dim, unsigned int y_dim);
	~GameWorld ();
	Position size ();
	std::unordered_map<std::string, GameObject> tanks_;
	StepReturn step (Positions pos);
	std::string addTank (const GameObject &);
	bool addProjectile (Position);
	std::vector<Positions> allProjectilesStep ();
	[[nodiscard]] GameObject::Type typeAt (Position);
	static std::string &my_uuid ()
	{
		static std::string my_uuid;
		return my_uuid;
	}
	bool playerAlive = true;

private:
	[[nodiscard]] GameObject::Type &at (Position);
	StepReturn tankStep (const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos);
	StepReturn projectileStep (const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos);
	MyGameWorldField field_;
	ManagedVector<Position> projectiles_;
	ManagedVector<Positions> playerPosChanges_;
	ManagedVector<Position> newProjectiles_;
	friend WorldGenerator;
};


#endif//SSDL_GAMEWORLD_H
