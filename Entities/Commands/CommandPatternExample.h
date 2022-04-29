//
// Created by true on 2022-04-27.
//

#ifndef SSDL_COMMANDPATTERNEXAMPLE_H
#define SSDL_COMMANDPATTERNEXAMPLE_H
#include "../GameObjects/GameObjectExample.h"
#include "../Errors/ErrorCode.h"

class CommandPatternExample {
public:
    CommandPatternExample() {};
	virtual ~CommandPatternExample();

	virtual SdlErrorCodeExample execute() = 0;
};


#endif //SSDL_COMMANDPATTERNEXAMPLE_H
