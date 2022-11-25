//
// Created by true on 2022-04-29.
//

#include <iostream>
#include "Game.h"


// Движение на стрелки, стрелять на пробел
Game::Game(): syncStreamErrors_(std::cerr),
#ifdef MAKE_LOG
				logFileForEverything_("1.txt")
				, logsSynchroStream_(logFileForEverything_),
				renderer_(isCurrentlyWorking_,logsSynchroStream_),
				processor_(WorldGenerator::generateWorld("labirinth.txt"),logsSynchroStream_)
#else
              renderer_(isCurrentlyWorking_),
              processor_(WorldGenerator::generateWorld("labirinth.txt"))
#endif
{
	isCurrentlyWorking_.store(false);
	renderer_.setProcessor(&processor_);
}

Game::~Game() = default;
void Game::mainLoop () {
// Преобразую
// От SDL ивентов образуется набор команд, которые обрабатываются в потоке процессора
// Обработка движения снарядов происходит на каждом шагу(а неплохо бы это в потоке процессинга делать)
// Для этого необходимо в потоке процессинга после обработки движения скидывать миру координаты новых позиций снарядов.
// Кроме того, необходимо сделать так, чтобы обработка движения частиц происходила лишь один раз за обновление мира.
// (обеспечение синхронизации)
//TODO P.S. Все проверки должны быть убраны из метода update, это игровой цикл, он критичен к производительности.
//TODO P.P.S. Проверки должны производится в месте, обрабатывающем возможность перемещения, при добавлении в очередь
// (в отдельном потоке, при добавлении миру в очередь обновленных данных)
	while(isCurrentlyWorking_.load()){
		update();
	}
}

void Game::start() {
	if (!isCurrentlyWorking_.load()) {
		isCurrentlyWorking_.store(true);
		renderer_.prepare();
		thProcessingEvents_ = std::jthread([&](){ renderer_.processingEventsLoop();});
		mainLoop();
	}
	else{
		syncStreamErrors_ << "Trying to start work, when already working.";
		syncStreamErrors_.emit();
	}
}

void Game::stop() {
	if (isCurrentlyWorking_.load()) {
		isCurrentlyWorking_.store(false);
		thProcessingEvents_.join();
	}
	else{
		syncStreamErrors_ << "Trying to stop working, when already not such busy.";
		syncStreamErrors_.emit();
	}
}

void Game::update ()
{
	processor_.processProjectilesMoving();
	processor_.processNpc();
	processor_.processCommands();
	renderer_.render();
	sched_yield();
}
