//
// Created by true on 2022-04-27.
//

#ifndef SSDL_BASECOMMAND_H
#define SSDL_BASECOMMAND_H
#include "../GameObjects/BaseGameObject.h"
#include "../Errors/ErrorCode.h"
#include "../GameObjects/World/GameWorld.h"

struct BaseCommand {
	enum class Type{
		UNDEFINED,
		MOVE_COMMAND,
		SHOOT_COMMAND
	};
//	BaseCommand() ;
//    BaseCommand(BaseGameObject*, Type) ;
//	virtual ~BaseCommand();

//	virtual SdlErrorCodeExample execute() = 0;
	BaseGameObject *obj_;
	Type type_;
	Position position_;
};


#endif //SSDL_BASECOMMAND_H
