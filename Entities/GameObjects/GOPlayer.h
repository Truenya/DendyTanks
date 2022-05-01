//
// Created by true on 2022-04-28.
//

#ifndef SSDL_GOPLAYER_H
#define SSDL_GOPLAYER_H

#include "World/MyGameWorld.h"

class GOPlayer : public BaseGameObject{
public:
	virtual ~GOPlayer();

	GOPlayer(int x,int y, BaseGameObject::Type typ);
};


#endif //SSDL_GOPLAYER_H
