//
// Created by true on 2022-04-28.
//

#ifndef SSDL_MYGAMEWORLD_H
#define SSDL_MYGAMEWORLD_H

#include "vector"
#include "memory"
#include "../BaseGameObject.h"
class WorldGenerator;
class CommandsProcessor;
//typedef std::unique_ptr<BaseGameObject> ptr_GamObjExample;
//typedef std::vector<std::vector<ptr_GamObjExample>> MyGameWorldField;
typedef std::vector<std::vector<BaseGameObject>> MyGameWorldField;
class MyGameWorld {
public:
	MyGameWorld( unsigned int  x_dim, unsigned int y_dim);
	~MyGameWorld();
	[[nodiscard]] BaseGameObject &at(Position) ;
	Position size();
	BaseGameObject *player_{};
private:
    MyGameWorldField field_;
	friend WorldGenerator;
	friend BaseGameObject;
	friend CommandsProcessor;
};


#endif //SSDL_MYGAMEWORLD_H
