//
// Created by true on 2022-04-25.
//

#ifndef SSDL_SDL_EXAMPLE_H
#define SSDL_SDL_EXAMPLE_H

#include <SDL2/SDL.h>
#include <iostream>
#include "Processors/WorldGenerator.h"
#include <chrono>

using namespace std::chrono;

/**
 * @brief Класс взаимодействия с SDL2 библиотекой.
 * @details Вся логика внутри, снаружи запускается только loop
 * @TODO вынести в разделяемую библиотеку(подпроект)
 */
class mySdlExample {

    int screenWidth_ = 1000;
    int screenHeight_ = 1000;
	coordinates dims;
	int rectSize = 25;
    SDL_Window *sdlWindowTest_;
    SDL_Renderer *sdlRenderer_;
    SDL_Surface *scr;
    SDL_Texture *sdlWallTexture_;
	SDL_Texture *sdlPlayerTexture_;
    SDL_Event e;
    bool init_();
	void fillMap();
    bool load_();
    int quit_();
    bool run = true;
    void processEvents(const int speed);//, int &x, int &y);
	std::unique_ptr<MyGameWorld> gameWorld_;
	time_t prevRender;
public:
    int loop();
	mySdlExample();

};


#endif //SSDL_SDL_EXAMPLE_H
