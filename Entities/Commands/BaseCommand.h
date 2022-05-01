//
// Created by true on 2022-04-27.
//

#ifndef SSDL_BASECOMMAND_H
#define SSDL_BASECOMMAND_H
#include "../GameObjects/BaseGameObject.h"
#include "../Errors/ErrorCode.h"
#include "../GameObjects/World/MyGameWorld.h"

struct BaseCommand {
	enum class Type{
		Undefined,
		MoveCommand,
		ShootCommand
	};
	BaseCommand() ;
    BaseCommand(BaseGameObject*, Type) ;
	virtual ~BaseCommand();

	virtual SdlErrorCodeExample execute() = 0;
	BaseGameObject *obj;
	Type type;
};


#endif //SSDL_BASECOMMAND_H
