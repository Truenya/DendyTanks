//
// Created by true on 2022-05-01.
//

#ifndef SSDL_SHOOTGAMEOBJECT_H
#define SSDL_SHOOTGAMEOBJECT_H

#include "BaseGameObject.h"

struct ShootGameObject : public  BaseGameObject{
	ShootGameObject(Position pos,std::vector<std::vector<BaseGameObject>> *field);
	~ShootGameObject() override;
	bool needExplosion_;
};


#endif //SSDL_SHOOTGAMEOBJECT_H
