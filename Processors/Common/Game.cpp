//
// Created by true on 2022-04-29.
//

#include <iostream>
#include "Game.h"


Game::Game(): syncStreamErrors_(std::cerr),
#ifdef MAKE_LOG
				logFileForEverything_("1.txt")
				, logsSynchroStream_(logFileForEverything_),
				renderer_(work_,logsSynchroStream_),
				processor_(WorldGenerator::generateWorld("labirinth.txt"),logsSynchroStream_)
#else
              renderer_(work_),
              processor_(WorldGenerator::generateWorld("labirinth.txt"))
#endif
{
	work_.store(false);
	renderer_.setProcessor(&processor_);
}

Game::~Game() = default;


void Game::mainLoop () {
// Преобразую
// От SDL ивентов образуется набор команд, которые обрабатываются в потоке процессора
// Мир обновляется сам по переданным из процессора очередям
// И передает данные напрямую в renderer
// Соответственно обработка движения снарядов происходит на каждом шагу(а неплохо бы это в потоке процессинга делать)
// Для этого необходимо в потоке процессинга после обработки движения скидывать миру координаты новых позиций снарядов.
// Кроме того, необходимо сделать так, чтобы обработка движения частиц происходила лишь один раз за обновление мира.
// P.S. Все проверки должны быть убраны из метода update, это игровой цикл, он критичен к производительности.
// P.P.S. Проверки должны производится в месте, где обрабатывается возможность перемещения.
// P.P.P.S. Зачем нам действительно обновлять мир в методе update, пусть там просто буферы миров меняются местами.
// P.P.P.P.S. Два буфера как-то маловато.
// Пусть будет кольцевой буфер(паттерн очередь событий) где будет 4 мира.
// Типа буферный мир для потока обработки, где мы осуществляем все изменения без блокировок и очередей
// и текущий, из которого рисуем.
// Тогда надо создать два мира и положить их в игру.
// Получится следующая логика
// Процессор работает с одним миром по указателю
// В методе update говорим процессору поменять указатель на буферный мир с указателем на текущий.
// В теме ниже - возможно синхронизация enum и не так нужна... Проверить влияние на стабильность работы и быстродействие
// Через атомарную переменную (enum(int)), говорящую, что надо обновить мир.
// В цикле потока обработки проверять её после каждого действия, которое нельзя прервать.
// Меняем местами указатели на миры, выставляем в переменную статус, что обновление завершено и продолжаем обработку.
// В потоке отрисовки если статус атомарной переменной говорит что надо обновить мир -
// ждем в цикле пока не завершится обновление.
	while(work_.load()){
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
	if (!work_.load()) {
		work_.store(true);
		renderer_.prepare();
//		thProcessingCommands_ = std::jthread([&]() { mainLoop(); });
		thProcessingEvents_ = std::jthread([&](){ renderer_.processingEventsLoop();});
//		sleep(2);
		mainLoop();
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
		thProcessingEvents_.join();
	}
	else{
		syncStreamErrors_ << "Trying to stop working, when already not such busy.";
		syncStreamErrors_.emit();
	}
}

