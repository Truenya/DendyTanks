//
// Created by true on 2022-04-27.
//

#ifndef SSDL_BASECOMMAND_H
#define SSDL_BASECOMMAND_H
#include "../GameObjects/GameObject.h"
#include "../Errors/ErrorCode.h"
#include "../GameObjects/World/GameWorld.h"
/**
* @brief Команды генерируются: 1. От пользовательского ввода; 2. От ИИ
*
*/
struct BaseCommand {
	enum class Type{
		UNDEFINED,
		// TODO Merge them
		PLAYER_MOVE_COMMAND,
		PLAYER_SHOOT_COMMAND,
		NPC_MOVE_COMMAND,
		NPC_SHOOT_COMMAND
	};
/**
* @brief Type of command
*
*/
	Type type_{Type::UNDEFINED};
/**
* @brief Любой объект мира полностью характеризуется своими координатами.
* @details Аналогично любую команду можно выразить типом команды и координатами.
*/
	Positions positions_;
};


#endif //SSDL_BASECOMMAND_H
