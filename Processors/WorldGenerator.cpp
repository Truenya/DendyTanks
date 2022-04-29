//
// Created by true on 2022-04-28.
//

#include "WorldGenerator.h"
#include <iostream>
MyGameWorld *WorldGenerator::generateWorld(const std::string& map_filepath) {
	std::ifstream map_file(map_filepath);
	std::string str((std::istreambuf_iterator<char>(map_file)),
	                std::istreambuf_iterator<char>());
	return parseFromString(str);
}

#include <list>
MyGameWorld *WorldGenerator::parseFromString(std::string raw_file) {
	int x = 0;
	int y = 0;
	int max_x = 0;
	std::list<std::pair<int,int>> wallCoordinates;
	std::list<std::pair<int,int>> spaceCoordinates;
	std::pair<int,int> playerCoordinates;
	for (const char& ch: raw_file){
		if (ch == '\n') {
			++y;
			if (max_x < x) max_x = x;
			x = 0;
		}
		else {
			if (ch == '#') {
				wallCoordinates.emplace_back(std::pair<int, int>{x, y});
				++x;
			}
			else if (ch == ' ') {
				spaceCoordinates.emplace_back(std::pair<int, int>{x, y});
				++x;
			}else if (ch == 'A'){
				playerCoordinates = {x,y};
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	//FIXME некорректное вычисление к-ва переводов строки
	auto world = new MyGameWorld(max_x, y);
	for (auto wall:wallCoordinates)
	{
		world->at(wall.first, wall.second)->setType(GameObjectExample::Type::Wall);
	}


	for (auto space:spaceCoordinates)
	{
		world->at(space.first, space.second)->setType(GameObjectExample::Type::Space);
	}

	world->at(playerCoordinates.first,playerCoordinates.second)->setType(GameObjectExample::Type::Player);

	return world;
}
