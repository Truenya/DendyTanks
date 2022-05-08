//
// Created by true on 2022-04-28.
//

#include "WorldGenerator.h"
#include <iostream>
#include "../../Entities/GameObjects/PlayerGameObject.h"
MyGameWorld &WorldGenerator::generateWorld(const std::string& map_filepath) {
	static MyGameWorld* ptr = nullptr;
	if (ptr != nullptr)
		return *ptr;
	std::ifstream map_file(map_filepath);
	std::string str((std::istreambuf_iterator<char>(map_file)),
	                std::istreambuf_iterator<char>());
	return parseFromString(str,ptr);
}

#include <list>
MyGameWorld &WorldGenerator::parseFromString(const std::string& raw_file, [[maybe_unused]] MyGameWorld* ptr) {
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
			}else if (ch == 'A'){
				player_coordinates = {x,y,0,Position::Direction::BOT};
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	static auto world = MyGameWorld(max_x, y);
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
	ptr = &world;
	return world;
}
