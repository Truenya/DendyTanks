//
// Created by true on 2022-04-27.
//

#ifndef SSDL_MOVECOMMANDEXAMPLE_H
#define SSDL_MOVECOMMANDEXAMPLE_H

#include "BaseCommand.h"
#include "../GameObjects/ObjectProps/Position.h"
#include "gtest/gtest.h"

struct MoveCommandExample : public BaseCommand{
	virtual ~MoveCommandExample();
    SdlErrorCodeExample execute() override;
	Position dPos_;

	MoveCommandExample(BaseGameObject *obj, Position dPos);
};


#endif //SSDL_MOVECOMMANDEXAMPLE_H
