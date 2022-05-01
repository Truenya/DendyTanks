//
// Created by true on 2022-04-25.
//

#ifndef SSDL_SDL_EXAMPLE_H
#define SSDL_SDL_EXAMPLE_H

#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include "../CommandsProcessor.h"
#include "../../Entities/Commands/BaseCommand.h"
#include "../Parsers/WorldGenerator.h"
#include "ParticlesSystem.h"

using namespace std::chrono;

/**
 * @brief Класс взаимодействия с SDL2 библиотекой.
 * @details Вся логика внутри, снаружи запускается только loop
 * @TODO вынести в разделяемую библиотеку(подпроект)
 */
static constexpr int MS_IN_SECOND = 1000;
class Renderer {
	enum class fpsChangeDirection{
		increment,
		decrement
	};
    int screenWidth_ = 1000;
    int screenHeight_ = 1000;
	Position dims;
	int rectSize = 25;
	int fps = 30;
	int mspf = MS_IN_SECOND / fps;
    SDL_Window *sdlWindowTest_;
    SDL_Renderer *sdlRenderer_;
    SDL_Surface *scr;
    SDL_Texture *sdlWallTexture_;
	SDL_Texture *sdlTankBottom;
	SDL_Texture *sdlTankTop;
	SDL_Texture *sdlTankLeft;
	SDL_Texture *sdlTankRight;
	SDL_Texture *sdlFillTexture_;
	SDL_Event e;
	SDL_Rect playerRect;
    bool init_();
	void fillMap();
    bool load_();
    int quit_();
    void processEvents();
	void updateFps(fpsChangeDirection direction);
	time_t prevRender;
	CommandsProcessor *processor_;
	ParticlesSystem particles_;
	std::atomic_bool &run_;
public:
    int loop();
	Renderer(std::atomic_bool& run);
	bool render();

	void setProcessor(CommandsProcessor *processor);
};


#endif //SSDL_SDL_EXAMPLE_H
