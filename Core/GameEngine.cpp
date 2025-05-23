#include "GameEngine.h"
#include <iostream>
#include <SDL2/SDL_image.h>

GameEngine::GameEngine()
    : window_(nullptr), renderer_(nullptr), running_(false), lastFrameTime_(0), targetFPS_(60.0f), frameDelay_(1000.0f / 60.0f) {
}

GameEngine::~GameEngine() {
    shutdown();
}

bool GameEngine::initialize(const std::string& title, const std::string& configPath) {
    // Load configuration
    if (!ConfigManager::getInstance().loadFromFile(configPath)) {
        std::cout << "Failed to load configuration, using defaults" << std::endl;
    }
    
    // Get window settings from config
    int windowWidth = ConfigManager::getInstance().getInt("window_width", 800);
    int windowHeight = ConfigManager::getInstance().getInt("window_height", 600);
    targetFPS_ = ConfigManager::getInstance().getFloat("fps_limit", 60.0f);
    frameDelay_ = 1000.0f / targetFPS_;
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_SHOWN
    );
    
    if (!window_) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    
    // Initialize resource manager
    if (!ResourceManager::getInstance().initialize(renderer_)) {
        std::cerr << "Failed to initialize resource manager" << std::endl;
        return false;
    }
    
    // Register services in the service locator
    ServiceLocator::getInstance().registerService<ConfigManager>(&ConfigManager::getInstance());
    ServiceLocator::getInstance().registerService<ResourceManager>(&ResourceManager::getInstance());
    ServiceLocator::getInstance().registerService<EntityManager>(&EntityManager::getInstance());
    
    // Set running flag
    running_ = true;
    
    // Initialize timing
    lastFrameTime_ = SDL_GetTicks();
    
    return true;
}

void GameEngine::run() {
    // Main game loop
    while (running_) {
        // Calculate delta time
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastFrameTime_) / 1000.0f;
        lastFrameTime_ = currentTime;
        
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running_ = false;
            }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderer_);
        
        // Update all systems
        for (auto& system : systems_) {
            system->update(deltaTime);
        }
        
        // Present renderer
        SDL_RenderPresent(renderer_);
        
        // Cap frame rate
        Uint32 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < frameDelay_) {
            SDL_Delay(static_cast<Uint32>(frameDelay_ - frameTime));
        }
    }
}

void GameEngine::shutdown() {
    // Clear systems
    systems_.clear();
    
    // Clear entity manager
    EntityManager::getInstance().clear();
    
    // Clean up resource manager
    ResourceManager::getInstance().cleanup();
    
    // Clean up SDL
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    
    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}