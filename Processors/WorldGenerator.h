//
// Created by true on 2022-04-28.
//

#ifndef SSDL_WORLDGENERATOR_H
#define SSDL_WORLDGENERATOR_H

#include "../Entities/GameObjects/World/MyGameWorld.h"
#include <string>
#include <fstream>
#include <sstream>

class WorldGenerator {
	static MyGameWorld* parseFromString(std::string);

public:
	static MyGameWorld* generateWorld(const std::string& map_filepath);
};


#endif //SSDL_WORLDGENERATOR_H
