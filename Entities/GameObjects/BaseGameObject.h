//
// Created by true on 2022-04-27.
//BaseGameObject::Type BaseGameObject::getType() const {


#ifndef SSDL_BASEGAMEOBJECT_H
#define SSDL_BASEGAMEOBJECT_H

#include "ObjectProps/Position.h"
#include "../Errors/ErrorCode.h"
#include <vector>

struct BaseGameObject {
	enum class Type{
		Undefined,
		Space,
		Player,
		Wall,
		Shoot
	};
	BaseGameObject() ;
	BaseGameObject(Position pos, Type typo,std::vector<std::vector<BaseGameObject>> *field);
//	BaseGameObject(int x, int y, Type typo);

	virtual ~BaseGameObject();
	SdlErrorCodeExample move(Position deltaPos);
	Type type;

	[[nodiscard]] const Positions &getPositions() const;

protected:
	Positions positions_;
	std::vector<std::vector<BaseGameObject>> *field_;
};


#endif //SSDL_BASEGAMEOBJECT_H
