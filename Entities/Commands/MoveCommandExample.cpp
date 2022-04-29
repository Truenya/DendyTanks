//
// Created by true on 2022-04-27.
//

#include "MoveCommandExample.h"

MoveCommandExample::MoveCommandExample(GameObjectExample *obj, Position dPos):
	obj_(obj),
	dPos_(dPos) {}

MoveCommandExample::~MoveCommandExample() = default;

SdlErrorCodeExample MoveCommandExample::execute() {
	return obj_->move(dPos_);
}
