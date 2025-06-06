//
// Created by true on 2022-04-29.
//

#ifndef GAME_H
#define GAME_H

#include <atomic>
#include <syncstream>
#include <thread>

// Forward declarations to avoid circular dependencies
class Renderer;
class MainProcessor;

/**
 * @class Game
 * @brief Main game class that manages the game loop and threads
 */
class Game
{
public:
	/**
     * @brief Constructor
     */
	Game ();

	/**
     * @brief Destructor
     */
	~Game ();

	/**
     * @brief Start the game
     */
	void start ();

	/**
     * @brief Static method to stop the game
     */
	static void stopGame ();

private:
	// Static atomic flag for controlling game execution
	static std::atomic_bool isCurrentlyWorking_;

	// Thread-safe error stream
	std::osyncstream syncStreamErrors_;

	// Game threads
	std::jthread thProcessingEvents_;
	std::jthread thProcessingCommands_;
	std::jthread thProcessingNpc;

	// Game loop methods
	void mainLoop ();
	void update ();

	// Logging
#ifdef MAKE_LOG
	std::ofstream logFileForEverything_;
	std::osyncstream logsSynchroStream_;
#endif

	// Game components
	Renderer *renderer_;
	MainProcessor *processor_;
};


#endif//GAME_H
