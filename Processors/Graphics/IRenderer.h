#pragma once

#include <atomic>
#include <functional>
#include "../Common/MainProcessor.h"

/**
 * @class IRenderer
 * @brief Интерфейс для всех рендереров
 * 
 * Этот интерфейс определяет общие методы, которые должны быть реализованы
 * всеми конкретными рендерерами (SDL2, OpenGL, Vulkan и т.д.)
 */
class IRenderer {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~IRenderer() = default;

    /**
     * @brief Подготовка рендерера к работе
     * @return true, если подготовка прошла успешно, false в противном случае
     */
    virtual bool prepare() = 0;

    /**
     * @brief Отрисовка текущего кадра
     * @return true, если отрисовка прошла успешно, false в противном случае
     */
    virtual bool render() = 0;

    /**
     * @brief Обработка событий ввода
     */
    virtual void processEvents() = 0;

    /**
     * @brief Цикл обработки событий
     */
    virtual void processingEventsLoop() = 0;

    /**
     * @brief Установка процессора для взаимодействия с игровой логикой
     * @param processor Указатель на процессор
     */
    virtual void setProcessor(MainProcessor* processor) = 0;

    /**
     * @brief Завершение работы рендерера
     */
    virtual void quit() = 0;

    /**
     * @brief Получение текущего FPS
     * @return Текущий FPS
     */
    virtual int getFps() const = 0;
};