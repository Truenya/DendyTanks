//
// Created by true on 2022-04-28.
//

#ifndef SSDL_MYGAMEWORLD_H
#define SSDL_MYGAMEWORLD_H

#include "vector"
#include "../GameObjectExample.h"
#include "memory"

typedef std::unique_ptr<GameObjectExample> ptr_GamObjExample;
typedef std::vector<std::vector<ptr_GamObjExample>> myGameWorldField;
typedef std::pair<int, int> coordinates;

class MyGameWorld {
public:
	MyGameWorld( unsigned int  x_dim, unsigned int y_dim);
	~MyGameWorld();
	[[nodiscard]] GameObjectExample *at(int , int) const;
	coordinates size();
private:
	myGameWorldField field_;
};


#endif //SSDL_MYGAMEWORLD_H
