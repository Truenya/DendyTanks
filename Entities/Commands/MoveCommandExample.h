//
// Created by true on 2022-04-27.
//

#ifndef SSDL_MOVECOMMANDEXAMPLE_H
#define SSDL_MOVECOMMANDEXAMPLE_H

#include "CommandPatternExample.h"
#include "../GameObjects/ObjectProps/Position.h"
#include "gtest/gtest.h"

class MoveCommandExample : CommandPatternExample{
private:
    MoveCommandExample(GameObjectExample *obj, Position dPos);
	~MoveCommandExample() override;
	Position dPos_;
public:
    SdlErrorCodeExample execute() override;
	GameObjectExample *obj_;
};


#endif //SSDL_MOVECOMMANDEXAMPLE_H
