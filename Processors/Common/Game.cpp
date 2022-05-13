//
// Created by true on 2022-04-29.
//

#include <iostream>
#include "Game.h"


// Движение на стрелки, стрелять на пробел, но стрелять не стоит ;D
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
// P.S. Все проверки должны быть убраны из метода update, это игровой цикл, он критичен к производительности.
// P.P.S. Проверки должны производится в месте, где обрабатывается возможность перемещения.
// (в отдельном потоке, при добавлении миру в очередь обновленных данных)
	while(isCurrentlyWorking_.load()){
		processor_.processParticlesMoving();
		processor_.processCommands();
		renderer_.render();
		sched_yield();
	}
}

void Game::start() {
//	auto cur_time = std::chrono::high_resolution_clock::now();
//	std::time_t t = std::chrono::system_clock::to_time_t(cur_time);
//	std::string ts = std::ctime(&t);
//	ts.resize(ts.size()-1);
//	logsSynchroStream_ << "Game started at: " << ts << '\n';
//	logsSynchroStream_.emit();
	if (!isCurrentlyWorking_.load()) {
		isCurrentlyWorking_.store(true);
		renderer_.prepare();
//		thProcessingCommands_ = std::jthread([&]() { mainLoop(); });
		thProcessingEvents_ = std::jthread([&](){ renderer_.processingEventsLoop();});
//		sleep(2);
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
//		th_ProcessingCommands_.join();
		thProcessingEvents_.join();
	}
	else{
		syncStreamErrors_ << "Trying to stop working, when already not such busy.";
		syncStreamErrors_.emit();
	}
}

