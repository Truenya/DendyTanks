//
// Created by true on 2022-04-25.
//

#ifndef SSDL_RENDERER_H
#define SSDL_RENDERER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include "../Common/CommandsProcessor.h"
#include "../../Entities/Commands/BaseCommand.h"
#include "../Parsers/WorldGenerator.h"
#include "../Common/ParticlesSystem.h"

using namespace std::chrono;

/**
 * @brief Класс взаимодействия с SDL2 библиотекой.
 * @details Вся логика внутри, снаружи запускается только processingEventsLoop
 * @TODO вынести в разделяемую библиотеку(подпроект)
 */
static constexpr int MS_IN_SECOND = 1000;
class Renderer {
	enum class FpsChangeDirection{
		INCREMENT,
		DECREMENT
	};
    int screenWidth_ = 1000;
    int screenHeight_ = 1000;
	Position worldSize_;
	int rectSize_ = 25;
	int fps_ = 30;
	int mspf_ = MS_IN_SECOND / fps_;
    SDL_Window *sdlWindowTest_;
    SDL_Renderer *sdlRenderer_;
//    SDL_Surface *scr;
    SDL_Texture *sdlWallTexture_;
	SDL_Texture *sdlTankBottomTextures_;
	SDL_Texture *sdlTankTopTextures_;
	SDL_Texture *sdlTankLeftTextures_;
	SDL_Texture *sdlTankRightTextures_;
	SDL_Texture *sdlExplosionTextures_;
	SDL_Texture *sdlFillTexture_;
	SDL_Event sdlEvent_;
	SDL_Rect playerRect_;
    bool init();
	bool load();
	void fillMap();
    int quit();
    void processEvents();
	void updateFps(FpsChangeDirection direction);
	void prepareTextures();
	time_t prevRender_;
	CommandsProcessor *processor_;
	std::atomic_bool &work_;
public:
	void prepare();
    void processingEventsLoop();
	Renderer(std::atomic_bool& run);
	bool render();
	bool makeSomePauseIfNeeded(const long int cur_time_ms);
	std::atomic_int_fast32_t renderTime_;

	void setProcessor(CommandsProcessor *processor);

	void renderPlayerMove();

	void renderPlayerShoots();

	void setScreenPosition (SDL_Rect &dstrect, int i, int j) const;

	void fillRectByPosition (SDL_Rect &dstrect, int i, int j) const;
};


#endif //SSDL_RENDERER_H
