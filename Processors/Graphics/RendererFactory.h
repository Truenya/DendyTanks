#pragma once

#include <memory>
#include <string>
#include "IRenderer.h"
#include "SDL2Renderer.h"

/**
 * @enum RendererType
 * @brief Типы доступных рендереров
 */
enum class RendererType {
    SDL2,
    // В будущем можно добавить другие типы: OpenGL, Vulkan и т.д.
};

/**
 * @class RendererFactory
 * @brief Фабрика для создания рендереров
 */
class RendererFactory {
public:
    /**
     * @brief Создание рендерера указанного типа
     * @param type Тип рендерера
     * @return Указатель на созданный рендерер
     */
    static std::unique_ptr<IRenderer> createRenderer(RendererType type) {
        switch (type) {
            case RendererType::SDL2:
#ifndef MAKE_LOG
                return std::make_unique<SDL2Renderer>();
#else
                return std::make_unique<SDL2Renderer>(logStream);
#endif
            default:
                throw std::runtime_error("Unknown renderer type");
        }
    }

#ifdef MAKE_LOG
    /**
     * @brief Установка потока для логирования
     * @param logStream Поток для логирования
     */
    static void setLogStream(std::osyncstream& logStream) {
        RendererFactory::logStream = logStream;
    }

private:
    static std::osyncstream& logStream;
#endif
};