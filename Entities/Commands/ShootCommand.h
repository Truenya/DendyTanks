//
// Created by true on 2022-05-01.
//

#ifndef SSDL_SHOOTCOMMAND_H
#define SSDL_SHOOTCOMMAND_H
#include "BaseCommand.h"

struct ShootCommand : public BaseCommand{
	ShootCommand(BaseGameObject* object, Type typo);
	~ShootCommand() override;
	SdlErrorCodeExample execute() override;
};


#endif //SSDL_SHOOTCOMMAND_H
