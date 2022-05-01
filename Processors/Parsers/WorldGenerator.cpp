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
MyGameWorld &WorldGenerator::parseFromString(const std::string& raw_file,MyGameWorld* ptr) {
	int x = 0;
	int y = 0;
	int max_x = 0;
	std::list<Position> wallCoordinates;
	std::list<Position> spaceCoordinates;
	Position playerCoordinates;
	for (const char& ch: raw_file){
		if (ch == '\n') {
			++y;
			if (max_x < x) max_x = x;
			x = 0;
		}
		else {
			if (ch == '#') {
				wallCoordinates.emplace_back(Position{x, y});
				++x;
			}
			else if (ch == ' ') {
				spaceCoordinates.emplace_back(Position{x, y});
				++x;
			}else if (ch == 'A'){
				playerCoordinates = {x,y};
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	static auto world = MyGameWorld(max_x, y);
	for (auto wall:wallCoordinates)
	{
		world.at(wall) = BaseGameObject(wall,BaseGameObject::Type::Wall,&world.field_);
	}

	for (auto space:spaceCoordinates)
	{
		world.at(space) = BaseGameObject(space,BaseGameObject::Type::Space,&world.field_);
	}

	world.at(playerCoordinates) = PlayerGameObject(playerCoordinates,&world.field_);
	world.player_ = &world.at(playerCoordinates);
	ptr = &world;
	return world;
}
