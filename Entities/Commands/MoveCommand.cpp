//
// Created by true on 2022-04-27.
//

#include "MoveCommand.h"

MoveCommand::MoveCommand(BaseGameObject *obj, Position dPos):
	BaseCommand(obj,Type::MoveCommand),
	dPos_(dPos){}

MoveCommand::~MoveCommand() = default;

SdlErrorCodeExample MoveCommand::execute() {

	return obj->move(dPos_);
}
