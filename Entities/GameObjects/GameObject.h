//
// Created by true on 2022-04-27.
//GameObject::Type GameObject::getType() const {


#ifndef SSDL_GAMEOBJECT_H
#define SSDL_GAMEOBJECT_H

#include "../Errors/ErrorCode.h"
#include "Position.h"
#include <vector>

struct  GameObject {
	enum class Type{
		UNDEFINED,
		SPACE,
		PLAYER,
		ENEMY,
		WALL,
		PROJECTILE
	};
	GameObject() ;
	GameObject(const Position &pos, Type typo);//,std::vector<std::vector<GameObject::Type>> *field);
	~GameObject();
	Type type_;
	[[nodiscard]] const Positions &getPositions() const;
	void step();
	void rotate(Position::Direction direction);
	// TODO size_t uid;
protected:
	Positions positions_;
//	std::vector<std::vector<GameObject::Type>> *field_;
//	friend struct ShootCommand;
//	friend struct MoveCommand;
};


#endif //SSDL_GAMEOBJECT_H
