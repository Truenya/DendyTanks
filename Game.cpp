#include "Game.h"
#include "Core/ConfigManager.h"
#include "Core/ResourceManager.h"
#include "Core/ECS/EntityManager.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/ECS/Components/RenderComponent.h"
#include "Core/ECS/Components/CollisionComponent.h"
#include <iostream>

// Initialize static member
std::atomic_bool Game::isRunning_{false};

// Static method to stop the game
void Game::stopGame() {
    isRunning_.store(false);
}

// Constructor
Game::Game() : engine_(std::make_unique<GameEngine>()) {
}

// Destructor
Game::~Game() {
    // Engine cleanup is handled by its destructor
}

// Initialize the game
bool Game::initialize() {
    // Initialize the game engine
    if (!engine_->initialize("Dendy Tanks", "config.ini")) {
        std::cerr << "Failed to initialize game engine" << std::endl;
        return false;
    }
    
    // Add systems
    engine_->addSystem<RenderSystem>(engine_->getRenderer());
    engine_->addSystem<CollisionSystem>();
    
    // Load resources
    auto& resourceManager = ResourceManager::getInstance();
    
    // Load textures
    resourceManager.loadTexture("tank_up", "tanks_t_green_blue_red_512x605.png");
    resourceManager.loadTexture("tank_right", "tanks_r_green_blue_red_512x605.png");
    resourceManager.loadTexture("tank_down", "tanks_b_green_blue_red_512x605.png");
    resourceManager.loadTexture("tank_left", "tanks_l_green_blue_red_512x605.png");
    resourceManager.loadTexture("wall", "fill.png");
    
    // Initialize game world
    initializeWorld();
    
    return true;
}

// Start the game
void Game::start() {
    if (!isRunning_.load()) {
        // Set game as running
        isRunning_.store(true);
        
        // Run the game loop
        engine_->run();
    } else {
        std::cerr << "Trying to start game when it's already running." << std::endl;
    }
}

// Initialize game world
void Game::initializeWorld() {
    // Get map file from config
    std::string mapFile = ConfigManager::getInstance().getString("map_file", "labirinth.txt");
    
    // Load map data
    std::string mapData = ResourceManager::getInstance().loadMapData(mapFile);
    
    // Create player
    createPlayer();
    
    // Create enemies
    createEnemies();
    
    // Create walls
    createWalls(mapData);
}

// Create player entity
void Game::createPlayer() {
    // Create player entity
    auto player = EntityManager::getInstance().createEntity("player");
    
    // Add components
    auto transform = player->addComponent<TransformComponent>(100, 100, TransformComponent::Direction::UP);
    auto render = player->addComponent<RenderComponent>("tank_up", 32, 32);
    auto collision = player->addComponent<CollisionComponent>(32, 32, true, "player");
    
    // Set source rectangle for rendering (first tank in the sprite sheet)
    render->setSourceRect(0, 0, 128, 128);
}

// Create enemy entities
void Game::createEnemies() {
    // Create enemy entity
    auto enemy = EntityManager::getInstance().createEntity("enemy1");
    
    // Add components
    auto transform = enemy->addComponent<TransformComponent>(300, 300, TransformComponent::Direction::DOWN);
    auto render = enemy->addComponent<RenderComponent>("tank_down", 32, 32);
    auto collision = enemy->addComponent<CollisionComponent>(32, 32, true, "enemy");
    
    // Set source rectangle for rendering (second tank in the sprite sheet)
    render->setSourceRect(128, 0, 128, 128);
}

// Create wall entities
void Game::createWalls(const std::string& mapData) {
    // Parse map data
    int x = 0;
    int y = 0;
    int wallCount = 0;
    
    for (char c : mapData) {
        if (c == '\n') {
            // New line
            y++;
            x = 0;
        } else if (c == '#') {
            // Wall
            std::string wallId = "wall" + std::to_string(wallCount++);
            auto wall = EntityManager::getInstance().createEntity(wallId);
            
            // Add components
            wall->addComponent<TransformComponent>(x * 32, y * 32);
            wall->addComponent<RenderComponent>("wall", 32, 32);
            wall->addComponent<CollisionComponent>(32, 32, true, "wall");
            
            x++;
        } else if (c == ' ' || c == '.') {
            // Empty space
            x++;
        }
    }
}