#pragma once

#include <atomic>
#include <functional>
#include "IRenderer.h"
#include "../Common/MainProcessor.h"

/**
 * @class BaseRenderer
 * @brief Базовый класс для всех рендереров
 * 
 * Этот класс содержит общую логику для всех рендереров
 */
class BaseRenderer : public IRenderer {
protected:
    MainProcessor* processor = nullptr;
    std::atomic<bool> isRunning{false};
    std::atomic<int> fps{0};

public:
    /**
     * @brief Конструктор
     */
    BaseRenderer() = default;

    /**
     * @brief Виртуальный деструктор
     */
    virtual ~BaseRenderer() = default;

    /**
     * @brief Установка процессора для взаимодействия с игровой логикой
     * @param processor Указатель на процессор
     */
    void setProcessor(MainProcessor* processor) override {
        this->processor = processor;
    }

    /**
     * @brief Получение текущего FPS
     * @return Текущий FPS
     */
    int getFps() const override {
        return fps;
    }

    /**
     * @brief Завершение работы рендерера
     */
    void quit() override {
        isRunning = false;
    }

protected:
    /**
     * @brief Обновление FPS
     * @param newFps Новое значение FPS
     */
    void updateFps(int newFps) {
        fps = newFps;
    }
};