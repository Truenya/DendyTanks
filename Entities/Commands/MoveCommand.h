//
// Created by true on 2022-04-27.
//

#ifndef SSDL_MOVECOMMAND_H
#define SSDL_MOVECOMMAND_H

#include "BaseCommand.h"
#include "../GameObjects/ObjectProps/Position.h"
#include "gtest/gtest.h"

struct MoveCommand : public BaseCommand{
	virtual ~MoveCommand();
    SdlErrorCodeExample execute() override;
	Position dPos_;

	MoveCommand(BaseGameObject *obj, Position dPos);
};


#endif //SSDL_MOVECOMMAND_H
