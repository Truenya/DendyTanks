//
// Created by true on 2022-05-01.
//

#include "ShootCommand.h"

ShootCommand::ShootCommand(BaseGameObject *object, BaseCommand::Type typo) : BaseCommand(object, typo) {

}

ShootCommand::~ShootCommand() {

}

SdlErrorCodeExample ShootCommand::execute() {
	return SdlErrorCodeExample(SdlErrorCodeExample::CANNOT_MOVE);
}
