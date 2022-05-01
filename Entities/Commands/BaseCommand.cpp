//
// Created by true on 2022-04-27.
//

#include "BaseCommand.h"

BaseCommand::~BaseCommand() {

}

BaseCommand::BaseCommand():obj(nullptr),type(Type::Undefined) {

}

BaseCommand::BaseCommand(BaseGameObject * gameObject, BaseCommand::Type comType): obj(gameObject), type(comType) {

}
