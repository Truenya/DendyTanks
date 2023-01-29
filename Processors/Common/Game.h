//
// Created by true on 2022-04-29.
//

#ifndef GAME_H
#define GAME_H

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
	static void sigHandler (int);
	static std::atomic_bool isCurrentlyWorking_;
private:
	std::osyncstream syncStreamErrors_;
	std::jthread thProcessingEvents_;
	std::jthread thProcessingCommands_;
	std::jthread thProcessingNpc;
	void mainLoop () ;
	void update();
#ifdef MAKE_LOG
	std::ofstream logFileForEverything_;
	std::osyncstream logsSynchroStream_;
#endif
	Renderer renderer_;
	MainProcessor processor_;
};


#endif//GAME_H
