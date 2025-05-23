#ifndef DENDY_TANKS_NEW_GAME_H
#define DENDY_TANKS_NEW_GAME_H

#include "Core/GameEngine.h"
#include "Core/ECS/Systems/RenderSystem.h"
#include "Core/ECS/Systems/CollisionSystem.h"
#include <memory>
#include <atomic>

/**
 * @class Game
 * @brief Main game class that uses the new architecture
 */
class Game {
public:
    /**
     * @brief Constructor
     */
    Game();
    
    /**
     * @brief Destructor
     */
    ~Game();
    
    /**
     * @brief Initialize the game
     * @return True if initialization was successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Start the game
     */
    void start();
    
    /**
     * @brief Static method to stop the game
     */
    static void stopGame();
    
private:
    // Game engine
    std::unique_ptr<GameEngine> engine_;
    
    // Static atomic flag for controlling game execution
    static std::atomic_bool isRunning_;
    
    // Initialize game world
    void initializeWorld();
    
    // Create player entity
    void createPlayer();
    
    // Create enemy entities
    void createEnemies();
    
    // Create wall entities
    void createWalls(const std::string& mapData);
};

#endif // DENDY_TANKS_NEW_GAME_H