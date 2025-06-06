//
// Created by true on 2022-04-25.
//

#ifndef SSDL_RENDERER_H
#define SSDL_RENDERER_H

#include "Processors/Common/FpsController.h"
#include "Processors/Common/MainProcessor.h"
#include "RenderData.h"
#include <SDL2/SDL.h>
#include <atomic>

/**
 * @class Renderer
 * @brief Class for interacting with the SDL2 library
 * @details All logic is contained within, only processingEventsLoop is called externally
 */
class Renderer
{
private:
	// Game components
	FpsController fps_;
	RenderData renderData_;
	std::atomic_bool &work_;
	MainProcessor *processor_;
	ManagedVector<std::pair<Position, int>> explosed_;
	bool rendered_{false};

	// Logging
#ifdef MAKE_LOG
	std::osyncstream &logsSynchroStream_;
#endif

	// Private methods
	bool init ();
	bool load ();
	void fillMap ();
	int quit ();
	void processEvents ();
	void prepareTextures ();

public:
	/**
     * @brief Constructor
     * @param run Reference to atomic flag controlling execution
     */
#ifndef MAKE_LOG
	explicit Renderer (std::atomic_bool &run);
#else
	explicit Renderer (std::atomic_bool &run, std::osyncstream &logStream);
#endif

	/**
     * @brief Destructor
     */
	~Renderer ();

	/**
     * @brief Prepare the renderer
     */
	void prepare ();

	/**
     * @brief Main event processing loop
     */
	void processingEventsLoop ();

	/**
     * @brief Render the current frame
     * @return True if rendering was successful
     */
	bool render ();

	/**
     * @brief Set the processor reference
     * @param processor Pointer to the MainProcessor
     */
	void setProcessor (MainProcessor *processor);

	/**
     * @brief Render player movement
     */
	void renderPlayerMove ();

	/**
     * @brief Render NPC movement
     */
	void renderNpcMove ();

	/**
     * @brief Render projectiles
     */
	void renderShoots ();

	/**
     * @brief Set screen position for rendering
     * @param dstrect Rectangle to position
     * @param i Row index
     * @param j Column index
     */
	void setScreenPosition (SDL_Rect &dstrect, int i, int j) const;

	/**
     * @brief Fill rectangle at position
     * @param dstrect Rectangle to fill
     * @param i Row index
     * @param j Column index
     */
	void fillRectByPosition (SDL_Rect &dstrect, int i, int j) const;
};

#endif//SSDL_RENDERER_H
