//
// Created by true on 2022-04-27.
//BaseGameObject::Type BaseGameObject::getType() const {


#ifndef SSDL_BASEGAMEOBJECT_H
#define SSDL_BASEGAMEOBJECT_H

#include "ObjectProps/Position.h"
#include "../Errors/ErrorCode.h"
#include <vector>

struct  BaseGameObject {
	enum class Type{
		UNDEFINED,
		SPACE,
		PLAYER,
		WALL,
		PROJECTILE
	};
	BaseGameObject() ;
	BaseGameObject(Position pos, Type typo,std::vector<std::vector<BaseGameObject>> *field);

	virtual ~BaseGameObject();
	SdlErrorCodeExample move(Position delta_pos);
	Type type_;

	[[nodiscard]] const Positions &getPositions() const;
	void step();
	void rotate(Position::Direction direction);
	void setDestinationPoint(const Position& pos);
protected:
	Positions positions_;
	std::vector<std::vector<BaseGameObject>> *field_;
	friend struct ShootCommand;
	friend struct MoveCommand;
};


#endif //SSDL_BASEGAMEOBJECT_H
