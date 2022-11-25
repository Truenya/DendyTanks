//
// Created by true on 2022-04-28.
//

#include "WorldGenerator.h"
#include <iostream>
#include "../../Entities/GameObjects/PlayerGameObject.h"
#include <cassert>
#include <list>

GameWorld &WorldGenerator::generateWorld(const std::string& map_filepath) {
	//bool WorldGenerator::existance = false;
	//GameWorld WorldGenerator::world;
	//if (existance)
	//	return world;
	std::ifstream map_file;
	map_file.open(map_filepath);
	assert(map_file.good());
	std::stringstream strStream;
	strStream << map_file.rdbuf();
	std::string str = strStream.str();
	std::string raw_file((std::istreambuf_iterator<char>(map_file)),
	                std::istreambuf_iterator<char>());

	int x = 0;
	int y = 0;
	int max_x = 0;
	std::list<Position> wall_coordinates;
	std::list<Position> space_coordinates;
	Position player_coordinates;
	for (const char& ch: raw_file){
		if (ch == '\n') {
			++y;
			if (max_x < x) max_x = x;
			x = 0;
		}
		else {
			if (ch == '#') {
				wall_coordinates.emplace_back(Position{x, y,0,Position::Direction::UNDEFINED});
				++x;
			}
			else if (ch == ' ') {
				space_coordinates.emplace_back(Position{x, y,0,Position::Direction::UNDEFINED});
				++x;
			}
			else if (ch == 'A') {
				player_coordinates = {x,y,0,Position::Direction::BOT};
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	world.init(max_x, y);
	for (auto wall:wall_coordinates)
	{
		world.at(wall) = BaseGameObject(wall,BaseGameObject::Type::WALL,&world.field_);
	}

	for (auto space:space_coordinates)
	{
		world.at(space) = BaseGameObject(space,BaseGameObject::Type::SPACE,&world.field_);
	}

	world.at(player_coordinates) = PlayerGameObject(player_coordinates,&world.field_);
	world.player_ = &world.at(player_coordinates);
	existance = true;
	return world;				

	return parseFromString(str);
}

/*GameWorld &WorldGenerator::parseFromString(const std::string& raw_file ) 
{
*	int x = 0;
	int y = 0;
	int max_x = 0;
	std::list<Position> wall_coordinates;
	std::list<Position> space_coordinates;
	Position player_coordinates;
	for (const char& ch: raw_file){
		if (ch == '\n') {
			++y;
			if (max_x < x) max_x = x;
			x = 0;
		}
		else {
			if (ch == '#') {
				wall_coordinates.emplace_back(Position{x, y,0,Position::Direction::UNDEFINED});
				++x;
			}
			else if (ch == ' ') {
				space_coordinates.emplace_back(Position{x, y,0,Position::Direction::UNDEFINED});
				++x;
			}
			else if (ch == 'A') {
				player_coordinates = {x,y,0,Position::Direction::BOT};
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	world.init(max_x, y);
	for (auto wall:wall_coordinates)
	{
		world.at(wall) = BaseGameObject(wall,BaseGameObject::Type::WALL,&world.field_);
	}

	for (auto space:space_coordinates)
	{
		world.at(space) = BaseGameObject(space,BaseGameObject::Type::SPACE,&world.field_);
	}

	world.at(player_coordinates) = PlayerGameObject(player_coordinates,&world.field_);
	world.player_ = &world.at(player_coordinates);
	existance = true;
	return world;
}*/
