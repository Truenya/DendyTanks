#ifndef DENDY_TANKS_GAME_ENGINE_H
#define DENDY_TANKS_GAME_ENGINE_H

#include <memory>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "ConfigManager.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "ECS/System.h"
#include "ECS/EntityManager.h"

/**
 * @class GameEngine
 * @brief Core game engine that manages the game loop and systems
 */
class GameEngine {
public:
    /**
     * @brief Constructor
     */
    GameEngine();
    
    /**
     * @brief Destructor
     */
    ~GameEngine();
    
    /**
     * @brief Initialize the game engine
     * @param title Window title
     * @param configPath Path to the configuration file
     * @return True if initialization was successful, false otherwise
     */
    bool initialize(const std::string& title, const std::string& configPath = "config.ini");
    
    /**
     * @brief Run the game loop
     */
    void run();
    
    /**
     * @brief Shut down the game engine
     */
    void shutdown();
    
    /**
     * @brief Add a system to the engine
     * @tparam T The system type
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Pointer to the added system
     */
    template<typename T, typename... Args>
    T* addSystem(Args&&... args) {
        auto system = std::make_unique<T>(std::forward<Args>(args)...);
        T* systemPtr = system.get();
        
        systems_.push_back(std::move(system));
        systemPtr->initialize();
        
        return systemPtr;
    }
    
    /**
     * @brief Get a system by type
     * @tparam T The system type
     * @return Pointer to the system, or nullptr if not found
     */
    template<typename T>
    T* getSystem() const {
        for (const auto& system : systems_) {
            T* result = dynamic_cast<T*>(system.get());
            if (result) {
                return result;
            }
        }
        return nullptr;
    }
    
    /**
     * @brief Get the SDL window
     * @return Pointer to the SDL window
     */
    SDL_Window* getWindow() const {
        return window_;
    }
    
    /**
     * @brief Get the SDL renderer
     * @return Pointer to the SDL renderer
     */
    SDL_Renderer* getRenderer() const {
        return renderer_;
    }
    
    /**
     * @brief Check if the engine is running
     * @return True if running, false otherwise
     */
    bool isRunning() const {
        return running_;
    }
    
    /**
     * @brief Stop the engine
     */
    void stop() {
        running_ = false;
    }

private:
    // SDL components
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    
    // Systems
    std::vector<std::unique_ptr<System>> systems_;
    
    // Running state
    bool running_;
    
    // Timing
    Uint32 lastFrameTime_;
    float targetFPS_;
    float frameDelay_;
};

#endif // DENDY_TANKS_GAME_ENGINE_H