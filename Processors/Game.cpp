//
// Created by true on 2022-04-29.
//

#include <iostream>
#include "Game.h"

Game::Game(): syncStreamErrors_(std::cerr),
              renderer_(work_),
              processor_(WorldGenerator::generateWorld("labirinth.txt"))
{
	work_.store(false);
	renderer_.setProcessor(&processor_);
}

Game::~Game() {

}


void Game::mainLoop_ () {
	while(work_.load())
	{
		if (processor_.processCommands())
			renderer_.render();
		else
			renderer_.render();
		// TODO else
		// сообщение об ошибке
		sched_yield();
	}
};

void Game::start() {
	if (!work_.load()) {
		work_.store(true);
//		th_ProcessingCommands_ = std::jthread([&]() { mainLoop_(); });
		th_Rendering_ = std::jthread([&](){renderer_.loop();});
		sleep(2);
		mainLoop_();
	}
	else{
		syncStreamErrors_ << "Trying to get work_, when already working.";
		syncStreamErrors_.emit();
	}
}

void Game::stop() {
	if (work_.load()) {
		work_.store(false);
//		th_ProcessingCommands_.join();
		th_Rendering_.join();
	}
	else{
		syncStreamErrors_ << "Trying to stop working, when already not such busy.";
		syncStreamErrors_.emit();
	}
}

void Game::init() {
	renderer_.setProcessor(&processor_);
}
