#include "Processors/Graphics/Renderer.h"
#include "Processors/Common/Game.h"
#include <csignal>
#include <memory>

// Signal handler function
void signalHandler(int signal) {
    Game::stopGame();
}

int main() {
    // Set up signal handlers
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Create game instance and start it
    auto game = std::make_unique<Game>();
    game->start();
    
    return 0;
}