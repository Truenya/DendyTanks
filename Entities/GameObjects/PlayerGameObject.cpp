//
// Created by true on 2022-04-28.
//

#include "PlayerGameObject.h"

PlayerGameObject::PlayerGameObject(Position pos,std::vector<std::vector<BaseGameObject>>* field):
BaseGameObject(pos,Type::PLAYER,field) {}

PlayerGameObject::~PlayerGameObject() = default;
