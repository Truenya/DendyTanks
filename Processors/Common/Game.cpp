//
// Created by true on 2022-04-29.
//

#include <iostream>
#include "Game.h"

std::atomic_bool Game::isCurrentlyWorking_;

void Game::sigHandler (int signal)
{
	isCurrentlyWorking_.store (false);
}

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

void Game::update ()
{
	static long long unsigned i{0};

	typedef std::chrono::high_resolution_clock Clock;

	auto t1 = Clock::now();
	processor_.processProjectilesMoving();
	auto t2 = Clock::now();
	processor_.processNpc();
	auto t3 = Clock::now();
	processor_.processCommands();
	auto t4 = Clock::now();
	renderer_.render();
	auto t5 = Clock::now();
	if (i % 10000 == 0) {
		std::cout << "shoots: " << t2 - t1 << '\n';
		std::cout << "moves: " << t3 - t2 << '\n';
		std::cout << "npc: " << t4 - t3 << '\n';
		std::cout << "render: " << t5 - t4 << '\n';
	}
	i++;
	sched_yield();
}
