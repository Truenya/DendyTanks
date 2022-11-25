#include "CRenderer.h"
#include <iostream>
#include "wchar.h"
#include <chrono>
void CRenderer::prepare (){
	init();
	load();
	frame_rater <60> treshold;
	while(true){
		std::cout << "FPS does not exel treshold";
		treshold.sleep(); 
	}	
}
void CRenderer::processingEventsLoop()
{

}
bool CRenderer::render(){

    /*const long int cur_time_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	renderPlayerMove();
	renderPlayerShoots();
	return makeSomePauseIfNeeded(cur_time_ms);*/
    return true;
}

void CRenderer:: init(){
	initscr();
	Position size(processor_->worldSize());
	WINDOW *win = newwin(size.x_, size.y_, 10, 10);
	refresh();
	for (int i = 0; i <  size.x_; ++i)
	{
		for (int j = 0; j < size.y_; ++j)
		{
			auto type = processor_->typeAt({i, j});
			if (type == BaseGameObject::Type::UNDEFINED)
			throw std::logic_error ("Trying to render unknown object");
			switch (type)
			{
			case BaseGameObject::Type::WALL:
				mvaddch(i, j, '#');
				break;
			case BaseGameObject::Type::PLAYER:
				addch(STAR);
				break;
			case BaseGameObject::Type::SPACE:
				mvaddch(i, j, ' ');
				break;
			default:
				throw std::logic_error ("Trying to render unknown object");
			}
			
		}
	}
	wrefresh(win);
	getch();
	endwin();
}
bool CRenderer:: load(){
	return true;
}
void CRenderer::setProcessor(MainProcessor *processor)
{
	processor_ = processor;
}
CRenderer::CRenderer (std::atomic_bool &running) :
	work_(running),
	processor_(nullptr)
{
    // 
}
/*void CRenderer::updateFps (CRenderer::Shift direction)
{
	if (renderData_.fps_ > 0 && renderData_.fps_ < 55)
	{
		if (direction == FpsChangeDirection::INCREMENT)
		{
			renderData_.fps_++;
			renderData_.fps_++;
		}
		if (direction == FpsChangeDirection::DECREMENT)
		{
			renderData_.fps_--;
		}
		renderData_.millisecondsPerFrame_ = MS_IN_SECOND / (renderData_.fps_ > 0 ? renderData_.fps_ : 1);
	}
}*/