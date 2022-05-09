//
// Created by true on 2022-04-25.
//

#ifndef SSDL_RENDERER_H
#define SSDL_RENDERER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <chrono>
#include <atomic>
#include "../Common/CommandsProcessor.h"
#include "../../Entities/Commands/BaseCommand.h"
#include "../Parsers/WorldGenerator.h"
#include "../Common/ParticlesSystem.h"
#include "RenderData.h"


/**
 * @brief Класс взаимодействия с SDL2 библиотекой.
 * @details Вся логика внутри, снаружи запускается только processingEventsLoop
 * @TODO вынести в разделяемую библиотеку(подпроект)
 */
class Renderer {
	enum class FpsChangeDirection{
		INCREMENT,
		DECREMENT
	};
	RenderData renderData_;
    bool init();
	bool load();
	void fillMap();
    int quit();
    void processEvents();
	void updateFps(FpsChangeDirection direction);
	void prepareTextures();
	std::atomic_bool &work_;
	CommandsProcessor *processor_;
public:
	void prepare();
    void processingEventsLoop();
	explicit Renderer(std::atomic_bool& run);
	bool render();
	bool makeSomePauseIfNeeded(long int cur_time_ms);

	void setProcessor(CommandsProcessor *processor);

	void renderPlayerMove();

	void renderPlayerShoots();

	void setScreenPosition (SDL_Rect &dstrect, int i, int j) const;

	void fillRectByPosition (SDL_Rect &dstrect, int i, int j) const;
};

#endif //SSDL_RENDERER_H
