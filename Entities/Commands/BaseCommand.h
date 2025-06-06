//
// Created by true on 2022-04-27.
//

#ifndef SSDL_BASECOMMAND_H
#define SSDL_BASECOMMAND_H
/**
* @brief Команды генерируются: 1. От пользовательского ввода; 2. От ИИ
*
*/
#include "Entities/GameObjects/Position.h"
#include <string>
struct BaseCommand {
	enum class Type
	{
		UNDEFINED,
		MOVE_COMMAND,
		SHOOT_COMMAND
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
	/**
	 * @brief Но для танчиков проще использовать хэшмапу с айдишниками
	 */
	std::string uid;
};


#endif//SSDL_BASECOMMAND_H
