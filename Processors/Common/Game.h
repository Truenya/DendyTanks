//
// Created by true on 2022-04-29.
//

#ifndef SSDL_GAME_H
#define SSDL_GAME_H

#include <syncstream>
#include <fstream>
#include "SDL2/SDL.h"
#include <queue>
#include <thread>
#include <functional>
#include "../Graphics/Renderer.h"
#include "MainProcessor.h"


struct Game {
	Game();
	~Game();
	void start();
	void stop();
private:
	std::atomic_bool work_;
	std::osyncstream syncStreamErrors_;
	std::jthread thProcessingEvents_;
	std::jthread thProcessingCommands_;
	void mainLoop () ;
#ifdef MAKE_LOG
	std::ofstream logFileForEverything_;
	std::osyncstream logsSynchroStream_;
#endif
	Renderer renderer_;
	MainProcessor processor_;
};


#endif //SSDL_GAME_H
