//
// Created by true on 2022-05-08.
//

#ifndef SSDL_RENDERDATA_H
#define SSDL_RENDERDATA_H

using namespace std::chrono;
static constexpr int MS_IN_SECOND = 1000;

struct RenderData{
	ssize_t screenWidth_ = 1000;
	ssize_t screenHeight_ = 1000;
	Position worldSize_ {0,0,0};
	ssize_t rectSize_ = 25;
	int fps_ = 30;
	int millisecondsPerFrame_ = MS_IN_SECOND / fps_;
	SDL_Window *sdlWindowTest_ = nullptr;
	SDL_Renderer *sdlRenderer_ = nullptr;
	SDL_Texture *sdlWallTexture_ = nullptr;
	SDL_Texture *sdlTankBottomTextures_ = nullptr;
	SDL_Texture *sdlTankTopTextures_ = nullptr;
	SDL_Texture *sdlTankLeftTextures_ = nullptr;
	SDL_Texture *sdlTankRightTextures_ = nullptr;
	SDL_Texture *sdlExplosionTextures_ = nullptr;
	SDL_Texture *sdlFillTexture_ = nullptr;
	SDL_Event sdlEvent_;
	SDL_Rect playerRect_{0,0,0,0};
	time_t prevRender_ = 0;
};
#endif //SSDL_RENDERDATA_H
