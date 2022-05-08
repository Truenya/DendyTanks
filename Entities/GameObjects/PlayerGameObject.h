//
// Created by true on 2022-04-28.
//

#ifndef SSDL_PLAYERGAMEOBJECT_H
#define SSDL_PLAYERGAMEOBJECT_H

#include "BaseGameObject.h"

class PlayerGameObject : public BaseGameObject{
public:
	~PlayerGameObject() override;

	PlayerGameObject(Position position, std::vector<std::vector<BaseGameObject>> *field);
};


#endif //SSDL_PLAYERGAMEOBJECT_H
