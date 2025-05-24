#pragma once

#include <SDL2/SDL.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <stdexcept>

#include "BaseRenderer.h"
#include "RenderData.h"
#include "../Common/ResourceManager.h"
#include "../Common/FpsController.h"
#include "../Parsers/WorldGenerator.h"
#include "../../Entities/Commands/BaseCommand.h"

/**
 * @class SDL2Renderer
 * @brief Реализация рендерера на базе SDL2
 */
class SDL2Renderer : public BaseRenderer {
private:
    // Game components
    FpsController fpsController;
    RenderData renderData;
    ManagedVector<std::pair<Position,int>> explosed;
    bool rendered{false};
    
    // Logging
#ifdef MAKE_LOG
    std::osyncstream &logsSynchroStream;
#endif

    // Private methods
    bool init();
    bool load();
    void fillMap();
    void processSDLEvents();
    void prepareTextures();
    
    // SDL specific methods
    void setScreenPosition(SDL_Rect &dstrect, int i, int j) const;
    void fillRectByPosition(SDL_Rect &dstrect, int i, int j) const;
    void renderPlayerMove();
    void renderNpcMove();
    void renderShoots();

public:
    /**
     * @brief Конструктор
     */
#ifndef MAKE_LOG
    SDL2Renderer();
#else
    explicit SDL2Renderer(std::osyncstream& logStream);
#endif

    /**
     * @brief Деструктор
     */
    ~SDL2Renderer() override;

    /**
     * @brief Подготовка рендерера к работе
     * @return true, если подготовка прошла успешно, false в противном случае
     */
    bool prepare() override;

    /**
     * @brief Отрисовка текущего кадра
     * @return true, если отрисовка прошла успешно, false в противном случае
     */
    bool render() override;

    /**
     * @brief Обработка событий ввода
     */
    void processEvents() override;

    /**
     * @brief Цикл обработки событий
     */
    void processingEventsLoop() override;
};