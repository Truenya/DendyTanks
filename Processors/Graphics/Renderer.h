//
// Created by true on 2022-04-25.
//

#ifndef SSDL_RENDERER_H
#define SSDL_RENDERER_H

#include <SDL2/SDL.h>
#include "../../Entities/Commands/BaseCommand.h"
#include "../Common/MainProcessor.h"
#include "../Parsers/WorldGenerator.h"
#include "Processors/Common/FpsController.h"
#include "RenderData.h"
#include <atomic>
#include <chrono>
#include <iostream>
/**
 * @brief Класс взаимодействия с SDL2 библиотекой.
 * @details Вся логика внутри, снаружи запускается только processingEventsLoop
 * @TODO вынести в разделяемую библиотеку(подпроект)
 */
using namespace std::chrono;
class Renderer {
	FpsController fps_;
	RenderData renderData_;
    bool init();
	bool load();
	void fillMap();
    int quit();
    void processEvents();
	void prepareTextures();
	std::atomic_bool &work_;
	MainProcessor *processor_;
	ManagedVector<std::pair<Position,int>> explosed_;
	bool rendered_{false};
#ifdef MAKE_LOG
	std::osyncstream &logsSynchroStream_;
#endif
public:
	void prepare();
    void processingEventsLoop();
	~Renderer();
#ifndef MAKE_LOG
	explicit Renderer(std::atomic_bool& run);
#else
	explicit Renderer(std::atomic_bool& run,std::osyncstream &);
#endif
	bool render();
	void setProcessor(MainProcessor *processor);
	void renderPlayerMove();
	void renderNpcMove();
	void renderShoots();
	void setScreenPosition (SDL_Rect &dstrect, int i, int j) const;
	void fillRectByPosition (SDL_Rect &dstrect, int i, int j) const;
};

#endif //SSDL_RENDERER_H
