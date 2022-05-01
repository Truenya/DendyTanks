//
// Created by true on 2022-04-27.
//

#include "MoveCommandExample.h"

MoveCommandExample::MoveCommandExample(BaseGameObject *obj, Position dPos):
	BaseCommand(obj,Type::MoveCommand),
	dPos_(dPos){}

MoveCommandExample::~MoveCommandExample() = default;

SdlErrorCodeExample MoveCommandExample::execute() {

	return obj->move(dPos_);
}
