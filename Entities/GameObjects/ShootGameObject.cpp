//
// Created by true on 2022-05-01.
//

#include "ShootGameObject.h"

ShootGameObject::~ShootGameObject() {

}

ShootGameObject::ShootGameObject(Position pos, std::vector<std::vector<BaseGameObject>> *field): BaseGameObject(pos,Type::Shoot,field), needExplosion(true) {

}
