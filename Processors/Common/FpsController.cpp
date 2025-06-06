//
// Created by true on 1/27/23.
//

#include "FpsController.h"
#include <chrono>
#include <thread>

bool FpsController::makeSomePauseIfNeeded (const long CUR_TIME_MS)
{
	// С прошлой отрисовки прошло сколько-то милисекунд
	const long int MS_TIME_FROM_LAST_RENDER = CUR_TIME_MS - prevRender_;
	if (MS_TIME_FROM_LAST_RENDER < millisecondsPerFrame_)// Если прошло недостаточно времени с прошлой отрисовки
	{
		if (MS_TIME_FROM_LAST_RENDER <= 0) return false;// Если прошлая отрисовка не была проведена позади в будущем ;D
		const long int MILISECONDS_DELAY = millisecondsPerFrame_ - MS_TIME_FROM_LAST_RENDER;
		if (MILISECONDS_DELAY < 1000) {
			std::this_thread::sleep_for (std::chrono::milliseconds (MILISECONDS_DELAY));
		}
		updateFps (FpsChangeDirection::INCREMENT);// Значит можно рисовать чаще
		return false;
	}
	else {
		prevRender_ = duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now ().time_since_epoch ()).count ();
		updateFps (FpsChangeDirection::DECREMENT);
	}
	return true;
}

void FpsController::updateFps (FpsChangeDirection direction)
{
	if (fps_ > 0 && fps_ <= maxFps_)
	{
		fps_ += int (direction);
		millisecondsPerFrame_ = MS_IN_SECOND / fps_;
	}
}
