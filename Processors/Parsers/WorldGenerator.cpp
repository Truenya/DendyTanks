//
// Created by true on 2022-04-28.
//

#include "WorldGenerator.h"
#include <iostream>
GameWorld &WorldGenerator::generateWorld(const std::string& map_filepath) {
	static GameWorld* ptr = nullptr;
	if (ptr != nullptr)
		return *ptr;
	std::ifstream map_file(map_filepath);
	std::string str((std::istreambuf_iterator<char>(map_file)),
	                std::istreambuf_iterator<char>());
	return parseFromString(str,ptr);
}

#include <list>
GameWorld &WorldGenerator::parseFromString(const std::string& raw_file, [[maybe_unused]] GameWorld* ptr) {
	int x = 0;
	int y = 0;
	int max_x = 0;
	std::list<Position> wall_coordinates;
	std::list<Position> space_coordinates;
	Position player_coordinates;
	std::list<Position> enemy_coordinates;
	for (const char& ch: raw_file){
		if (ch == '\n') {
			++y;
			if (max_x < x) max_x = x;
			x = 0;
		}
		else {
			if (ch == '#') {
				wall_coordinates.emplace_back(Position{x, y,0,Position::Direction::EQUAL});
				++x;
			}
			else if (ch == ' ') {
				space_coordinates.emplace_back(Position{x, y,0,Position::Direction::EQUAL});
				++x;
			}
			else if (ch == 'A') {
				player_coordinates = {x,y,0,Position::Direction::BOT};
			}
			else if (ch == 'B') {
				enemy_coordinates.emplace_back(Position{x,y,0,Position::Direction::BOT});
			}
			else{
				std::cerr<<"Некорректный символ:" << ch;
			}
		}
	}
	static auto world = GameWorld(max_x, y);
	for (const auto &wall:wall_coordinates)
	{
		world.at(wall) = GameObject::Type::WALL;
	}

	for (const auto &space:space_coordinates)
	{
		world.at(space) = GameObject::Type::SPACE;
	}

	for (const auto &enemy:enemy_coordinates)
	{
		world.at(enemy) = GameObject::Type::ENEMY;
		world.addTank({enemy, GameObject::Type::ENEMY});
	}

	world.at(player_coordinates) = GameObject::Type::PLAYER;
	world.addTank ({player_coordinates,GameObject::Type::PLAYER});
	world.player_ = GameObject({player_coordinates,GameObject::Type::PLAYER});//, &world.field_);
	ptr = &world;
	return world;
}

