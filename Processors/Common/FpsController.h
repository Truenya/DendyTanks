//
// Created by true on 1/27/23.
//

#ifndef FPSCONTROLLER_H
#define FPSCONTROLLER_H

#include "time.h"

static constexpr int MS_IN_SECOND = 1000;

struct FpsController {
	bool makeSomePauseIfNeeded (long);
	int fps_ = 15;

private:
	enum class FpsChangeDirection
	{
		INCREMENT = 1,
		DECREMENT = -1
	};
	void updateFps (FpsChangeDirection direction);
	int millisecondsPerFrame_ = MS_IN_SECOND / fps_;
	time_t prevRender_ = time (nullptr);
	const int maxFps_ = 15;
};


#endif//FPSCONTROLLER_H
