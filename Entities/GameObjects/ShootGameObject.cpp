//
// Created by true on 2022-05-01.
//

#include "ShootGameObject.h"

ShootGameObject::~ShootGameObject() = default;

ShootGameObject::ShootGameObject(Position pos, std::vector<std::vector<BaseGameObject>> *field):
BaseGameObject(pos, Type::PROJECTILE, field), needExplosion_(true) {

}
