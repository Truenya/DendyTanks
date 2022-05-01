//
// Created by true on 2022-04-28.
//

#ifndef SSDL_MYGAMEWORLD_H
#define SSDL_MYGAMEWORLD_H

#include "vector"
#include "memory"
class MyGameWorld;
#include "../BaseGameObject.h"
#include "../../../Processors/Parsers/WorldGenerator.h"
//typedef std::unique_ptr<BaseGameObject> ptr_GamObjExample;
//typedef std::vector<std::vector<ptr_GamObjExample>> myGameWorldField;
typedef std::vector<std::vector<BaseGameObject>> myGameWorldField;
class MyGameWorld {
public:
	MyGameWorld( unsigned int  x_dim, unsigned int y_dim);
	~MyGameWorld();
	[[nodiscard]] BaseGameObject &at(Position) ;
	Position size();
	BaseGameObject *player_;
private:
    myGameWorldField field_;
	friend WorldGenerator;
	friend BaseGameObject;
};


#endif //SSDL_MYGAMEWORLD_H
