//
// Created by true on 2022-04-29.
//

#ifndef SSDL_GAME_H
#define SSDL_GAME_H
#include "SDL2/SDL.h"
#include <queue>
#include <thread>
#include <functional>
#include <syncstream>
#include "../Graphics/Renderer.h"
#include "CommandsProcessor.h"

struct Game {
	Game();
	~Game();
	void start();
	void stop();
private:
	std::atomic_bool work_;
	std::osyncstream syncStreamErrors_;
	Renderer renderer_;
	CommandsProcessor processor_;
//	std::jthread th_ProcessingCommands_;
	std::jthread thProcessingEvents_;
	void mainLoop () ;
};


#endif //SSDL_GAME_H
