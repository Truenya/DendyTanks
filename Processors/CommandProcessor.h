//
// Created by true on 2022-04-29.
//

#ifndef SSDL_COMMANDPROCESSOR_H
#define SSDL_COMMANDPROCESSOR_H
#include "SDL2/SDL.h"
#include <queue>
#include <memory>
#include "../Entities/Commands/CommandPatternExample.h"

class CommandProcessor {
public:
	CommandProcessor();
	~CommandProcessor();
	void addCommand(CommandPatternExample* command);
	std::priority_queue<std::pair<std::unique_ptr<CommandPatternExample>,long int>> s;
	
int a;
private:
	SDL_Renderer *sdlRenderer_;
	SDL_Surface *scr;
	SDL_Texture *sdlWallTexture_;
	SDL_Texture *sdlPlayerTexture_;
};


#endif //SSDL_COMMANDPROCESSOR_H
