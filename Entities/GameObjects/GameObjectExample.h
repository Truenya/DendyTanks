//
// Created by true on 2022-04-27.
//GameObjectExample::Type GameObjectExample::getType() const {


#ifndef SSDL_GAMEOBJECTEXAMPLE_H
#define SSDL_GAMEOBJECTEXAMPLE_H


#include "ObjectProps/Position.h"
#include "../Errors/ErrorCode.h"

class GameObjectExample {
public:
	enum class Type{
		Undefined,
		Space,
		Player,
		Wall
	};
	GameObjectExample() ;
	GameObjectExample(int x, int y, Type typo);

	virtual ~GameObjectExample();
	SdlErrorCodeExample move(Position deltaPos);

	void setType(Type type);

protected:
	Position pos_;
	Position prevPos_;
	Type type_;
public:
	Type getType() const;
};


#endif //SSDL_GAMEOBJECTEXAMPLE_H
