//
// Created by true on 2022-04-29.
//

#include "Game.h"
#include "../Graphics/Renderer.h"
#include "../Parsers/WorldGenerator.h"
#include "MainProcessor.h"
#include <iostream>

// Initialize static member
std::atomic_bool Game::isCurrentlyWorking_{false};

// Static method to stop the game
void Game::stopGame ()
{
	isCurrentlyWorking_.store (false);
}

// Constructor
Game::Game () :
    syncStreamErrors_ (std::cerr)
#ifdef MAKE_LOG
    ,
    logFileForEverything_ ("game_log.txt"), logsSynchroStream_ (logFileForEverything_)
#endif
{
	// Initialize game components
#ifdef MAKE_LOG
	renderer_ = new Renderer (isCurrentlyWorking_, logsSynchroStream_);
	processor_ = new MainProcessor (
	        WorldGenerator::generateWorld ("../resources/labirinth20x20.txt"),
	        logsSynchroStream_);
#else
	renderer_ = new Renderer (isCurrentlyWorking_);
	processor_ = new MainProcessor (
	        WorldGenerator::generateWorld ("../resources/labirinth20x20.txt"));
#endif

	// Print helpful message about resources
	std::cout << "Game initialized. Using resources from the resources directory." << std::endl;
	std::cout << "If you encounter any issues with resources, make sure the following files exist:" << std::endl;
	std::cout << "  - Map file: resources/labirinth20x20.txt or resources/labirinth50x50.txt" << std::endl;
	std::cout << "  - Texture files: resources/cvetok.png, resources/fill.png" << std::endl;
	std::cout << "  - Tank textures: resources/tanks_b_green_blue_red_512x605.png, etc." << std::endl;

	// Connect components
	renderer_->setProcessor (processor_);
}

// Destructor
Game::~Game ()
{
	// Ensure game is stopped
	isCurrentlyWorking_.store (false);

	// Clean up resources
	delete renderer_;
	delete processor_;
}

// Main game loop
void Game::mainLoop ()
{
	// Game loop runs until isCurrentlyWorking_ is set to false
	while (isCurrentlyWorking_.load ()) {
		update ();
	}
}

// Start the game
void Game::start ()
{
	if (isCurrentlyWorking_.load ()) {
		// Log error if already running
		syncStreamErrors_ << "Trying to start game when it's already running.";
		syncStreamErrors_.emit ();
		return;
	}
	// Prepare renderer
	renderer_->prepare ();

	// Set game as running
	isCurrentlyWorking_.store (true);

	// Start threads
	thProcessingEvents_ = std::jthread ([this] () {
		renderer_->processingEventsLoop ();
	});

	thProcessingNpc = std::jthread ([this] () {
		processor_->processingNpcLoop (isCurrentlyWorking_);
	});

	// Start main loop
	mainLoop ();
}

// Update game state
void Game::update ()
{
#ifdef DEBUG
	// Performance tracking
	using Clock = std::chrono::high_resolution_clock;

	// Process game logic
	auto t1 = Clock::now ();
#endif
	processor_->processProjectilesMoving ();
#ifdef DEBUG
	auto t2 = Clock::now ();
#endif
	processor_->processCommands ();
#ifdef DEBUG
	auto t3 = Clock::now ();
#endif
	renderer_->render ();
#ifdef DEBUG
	auto t4 = Clock::now ();

	// Performance logging (only every 5000 frames)
	static unsigned long long frameCount{0};

	if (frameCount % 5000 == 1) {
		auto projectileTime = std::chrono::duration_cast<std::chrono::microseconds> (t2 - t1);
		auto commandTime = std::chrono::duration_cast<std::chrono::microseconds> (t3 - t2);
		auto renderTime = std::chrono::duration_cast<std::chrono::microseconds> (t4 - t3);

		std::cout << "Performance metrics (microseconds):\n"
		          << "  Projectiles: " << projectileTime.count () << "\n"
		          << "  Commands: " << commandTime.count () << "\n"
		          << "  Rendering: " << renderTime.count () << "\n";
	}

	frameCount++;

#endif
	// Yield to other threads
	std::this_thread::yield ();
}
