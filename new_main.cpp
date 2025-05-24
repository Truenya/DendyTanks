#include "Game.h"
#include <csignal>
#include <memory>
#include <iostream>

// Signal handler function
void signalHandler(int signal) {
    std::cout << "Received signal " << signal << ", stopping game..." << std::endl;
    Game::stopGame();
}

int main() {
    // Set up signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Create game instance
    auto game = std::make_unique<Game>();
    
    // Initialize the game
    if (!game->initialize()) {
        std::cerr << "Failed to initialize game" << std::endl;
        return 1;
    }
    
    // Start the game
    game->start();
    
    return 0;
}