//
// Created by true on 2022-04-27.
//

#include "MoveCommand.h"

MoveCommand::MoveCommand(BaseGameObject *obj, Position d_pos):
	BaseCommand(obj,Type::MOVE_COMMAND),
	dPos_(d_pos){}

MoveCommand::~MoveCommand() = default;

SdlErrorCodeExample MoveCommand::execute() {
	return obj_->move(dPos_);
}
