//
// Created by true on 2022-04-25.
//
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include "../../Entities/Commands/BaseCommand.h"

void Renderer::processingEventsLoop ()
{
	while (work_.load())
	{
		processEvents();
	}
}

bool Renderer::render ()
{
	// Замерим время выполнения
	const long int cur_time_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	renderPlayerMove();
	renderPlayerShoots();
	return makeSomePauseIfNeeded(cur_time_ms);
}

bool Renderer::makeSomePauseIfNeeded (const long cur_time_ms)
{
	// С прошлой отрисовки прошло сколько-то милисекунд
	const long int MS_TIME_FROM_LAST_RENDER = cur_time_ms - renderData_.prevRender_;
	// Чаще чем 30 раз в секунду рендерить не будем
	if (MS_TIME_FROM_LAST_RENDER < renderData_.millisecondsPerFrame_)
	{
		// Если прошлая отрисовка не была проведена позади в будущем ;D
		if (MS_TIME_FROM_LAST_RENDER > 0)
		{	
			std::string frame(std::to_string (renderData_.fps_));
			auto fps = frame.c_str();
			SDLTest_DrawString(renderData_.sdlRenderer_,renderData_.screenWidth_-100,20, fps);
			const long int MILISECONDS_DELAY = renderData_.millisecondsPerFrame_ - MS_TIME_FROM_LAST_RENDER;
			updateFps(FpsChangeDirection::INCREMENT);
			rendered_ = false;
			if (MILISECONDS_DELAY < 1000)
			{
				SDL_Delay(MILISECONDS_DELAY);
				return false;
			}
		}
	}
	else
	{
		SDL_RenderPresent(renderData_.sdlRenderer_);
		renderData_.prevRender_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		updateFps(FpsChangeDirection::DECREMENT);
		rendered_ = true;
	}
	return true;
}

void Renderer::updateFps (Renderer::FpsChangeDirection direction)
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
}

void Renderer::processEvents ()
{
	while (SDL_PollEvent(&renderData_.sdlEvent_))
	{
		if (renderData_.sdlEvent_.type == SDL_QUIT) { work_.store(false);  return;}
		if (renderData_.sdlEvent_.type != SDL_KEYDOWN)	continue;
		const auto PRESSED_KEY = renderData_.sdlEvent_.key.keysym.sym;
		switch (PRESSED_KEY)
		{
//			case SDLK_UP: processor_->addPlayerCommand({processor_->getPlayer(),BaseCommand::Type::PLAYER_MOVE_COMMAND,
//					{0, -1, 0, Position::Direction::TOP}}); break;
//			case SDLK_DOWN: processor_->addPlayerCommand({processor_->getPlayer(), BaseCommand::Type::PLAYER_MOVE_COMMAND,
//					{0, 1, 0, Position::Direction::BOT}}); break;
//			case SDLK_RIGHT: processor_->addPlayerCommand({processor_->getPlayer(),BaseCommand::Type::PLAYER_MOVE_COMMAND,
//					{1, 0, 0, Position::Direction::RIGHT}}); break;
//			case SDLK_LEFT: processor_->addPlayerCommand({processor_->getPlayer(), BaseCommand::Type::PLAYER_MOVE_COMMAND,
//					{-1, 0, 0, Position::Direction::LEFT}}); break;
//			case SDLK_SPACE: processor_->addPlayerCommand({processor_->getPlayer(), BaseCommand::Type::SHOOT_COMMAND,{}});
//				break;
			case SDLK_UP: processor_->addPlayerCommand({BaseCommand::Type::PLAYER_MOVE_COMMAND,
						{{},{0, -1, 0, Position::Direction::TOP},{},{}}}); break;
			case SDLK_DOWN: processor_->addPlayerCommand({ BaseCommand::Type::PLAYER_MOVE_COMMAND,
						{{},{0, 1, 0, Position::Direction::BOT},{}, {}}}); break;
			case SDLK_RIGHT: processor_->addPlayerCommand({BaseCommand::Type::PLAYER_MOVE_COMMAND,
						{{},{1, 0, 0, Position::Direction::RIGHT},{},{}}}); break;
			case SDLK_LEFT: processor_->addPlayerCommand({BaseCommand::Type::PLAYER_MOVE_COMMAND,\
						{{},{-1, 0, 0, Position::Direction::LEFT},{},{}}}); break;
			case SDLK_SPACE: processor_->addPlayerCommand({ BaseCommand::Type::SHOOT_COMMAND,{}});
				break;
			default:
				continue;
		}
	}
}

#ifndef MAKE_LOG
Renderer::Renderer (std::atomic_bool &running) :
	work_(running),
	processor_(nullptr)
{
	renderData_.prevRender_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}
#else
Renderer::Renderer (std::atomic_bool &running,std::osyncstream &logs) :
		isCurrentlyWorking_(running),
		processor_(nullptr),
		logsSynchroStream_(logs)
{
	renderData_.prevRender_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count_();
}
#endif

bool Renderer::init ()
{
//    bool initialized = false;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Can't init: " << SDL_GetError() << std::endl;
		quit();
	}
	renderData_.worldSize_ = processor_->worldSize();

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	renderData_.screenWidth_ = dm.w - 80;
	renderData_.screenHeight_ = dm.h - 80;


	renderData_.sdlWindowTest_ = SDL_CreateWindow("Пробное окно SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			static_cast<int>(renderData_.screenWidth_), static_cast<int>(renderData_.screenHeight_), SDL_WINDOW_SHOWN);
	if (renderData_.sdlWindowTest_ == nullptr)
	{
		std::cout << "Can't create window: " << SDL_GetError() << std::endl;
		quit();
	}

	renderData_.sdlRenderer_ = SDL_CreateRenderer(renderData_.sdlWindowTest_, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderData_.sdlRenderer_ == nullptr)
	{
		std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
		quit();
	}
	static constexpr int WHITE = 0xFF;
	if (SDL_SetRenderDrawColor(renderData_.sdlRenderer_, WHITE, WHITE, WHITE, WHITE))
	{
		std::cout << "Can't fill renderer: " << SDL_GetError() << std::endl;
		quit();
	}

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags))
	{
		std::cout << "Can't init image: " << IMG_GetError() << std::endl;
		quit();
	}

	return false;
}

bool Renderer::load ()
{
//    bool isLoadedIncorrectly = false;

	SDL_Surface *temp_surf;
	temp_surf = IMG_Load("cvetok.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//	    isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlWallTexture_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	SDL_FreeSurface(temp_surf);

	if (renderData_.sdlWallTexture_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}

	temp_surf = IMG_Load("fill.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlFillTexture_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	SDL_FreeSurface(temp_surf);

	if (renderData_.sdlFillTexture_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	temp_surf = IMG_Load("tank_b.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlTankBottomTextures_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	if (renderData_.sdlTankBottomTextures_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}

	temp_surf = IMG_Load("tank_t.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlTankTopTextures_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	if (renderData_.sdlTankTopTextures_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}


	temp_surf = IMG_Load("tank_l.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlTankLeftTextures_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	if (renderData_.sdlTankLeftTextures_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}

	temp_surf = IMG_Load("tank_r.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlTankRightTextures_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	if (renderData_.sdlTankRightTextures_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}

	temp_surf = IMG_Load("explosion.png");
	if (temp_surf == nullptr)
	{
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	renderData_.sdlExplosionTextures_ = SDL_CreateTextureFromSurface(renderData_.sdlRenderer_, temp_surf);
	if (renderData_.sdlExplosionTextures_ == nullptr)
	{
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
//		isLoadedIncorrectly |= true;
		exit(-1);
	}
	fillMap();


	return false;
}

int Renderer::quit ()
{
	work_.store(false);
	SDL_DestroyWindow(renderData_.sdlWindowTest_);
	renderData_.sdlWindowTest_ = nullptr;

	SDL_DestroyRenderer(renderData_.sdlRenderer_);
	renderData_.sdlRenderer_ = nullptr;

	SDL_DestroyTexture(renderData_.sdlWallTexture_);

	SDL_Delay(1);
	SDL_Quit();
	SDL_Delay(1);
	IMG_Quit();
	SDL_Delay(1);
	return 0;
}

void Renderer::fillMap ()
{
	prepareTextures();

	SDL_Rect dstrect;
	for (int i = 0; i < renderData_.worldSize_.x_; ++i)
	{
		for (int j = 0; j < renderData_.worldSize_.y_; ++j)
		{
			fillRectByPosition(dstrect, i, j);
		}
	}
	SDL_RenderPresent(renderData_.sdlRenderer_);

	SDL_Delay(15);
}

void Renderer::fillRectByPosition (SDL_Rect &dstrect, int i, int j) const
{
	auto type = processor_->typeAt({i, j});
	if (type == BaseGameObject::Type::UNDEFINED)
		throw std::logic_error ("Trying to render unknown object");
	setScreenPosition(dstrect, i, j);
	switch (type)
	{
		case BaseGameObject::Type::WALL:
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlWallTexture_, nullptr, &dstrect);
			break;
		case BaseGameObject::Type::PLAYER:
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_, &renderData_.playerRect_, &dstrect);
			break;
		case BaseGameObject::Type::SPACE:
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &dstrect);
			break;
		default:
			throw std::logic_error ("Trying to render unknown object");
	}
}

void Renderer::setScreenPosition (SDL_Rect &dstrect, int i, int j) const
{
	dstrect.x = i * renderData_.rectSize_;
	dstrect.y = j * renderData_.rectSize_;
	dstrect.w = dstrect.h = renderData_.rectSize_;
}

void Renderer::prepareTextures ()
{
	SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, nullptr);
	renderData_.rectSize_ = (renderData_.screenHeight_ / renderData_.worldSize_.y_) - 1;
	SDL_Point player_texture_size;
	SDL_QueryTexture(renderData_.sdlTankBottomTextures_, NULL, NULL, &player_texture_size.x, &player_texture_size.y);
	renderData_.playerRect_.w = player_texture_size.x / 3 - 1;
	renderData_.playerRect_.h = player_texture_size.y / 3 - 1;
}
#include <list>
void Renderer::renderPlayerShoots ()
{
	SDL_Rect fillrect;
	SDL_Rect dstrect;
	dstrect.w = dstrect.h = fillrect.w = fillrect.h = renderData_.rectSize_;
	// TODO некрасиво, поправить
	for (size_t i = 0; i < explosed_.count() ; ++i)
	{
		if (!explosed_[i].second)
		{
			const Position pos = explosed_[i].first;
			fillrect.x = pos.x_ * renderData_.rectSize_;
			fillrect.y = pos.y_ * renderData_.rectSize_;
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &fillrect);
			explosed_.remove(i);
		}
		else{
			if (rendered_)
				explosed_[i].second--;
			else
				std::cout << "smth\n";
		}
	}
	auto shoots = processor_->getShoots();
	if(!shoots.size()) return;
	SDL_Point explosion_texture_size;
	SDL_QueryTexture(renderData_.sdlExplosionTextures_, NULL, NULL, &explosion_texture_size.x, &explosion_texture_size.y);

	SDL_Rect explosion_rect;
	SDL_Rect projectile_rect;
	explosion_rect.w = projectile_rect.w = explosion_texture_size.x / 5;
	explosion_rect.h = projectile_rect.h = explosion_texture_size.y / 2;
	explosion_rect.x = explosion_rect.w * 4;
	explosion_rect.y = projectile_rect.y = projectile_rect.x = 0;


	for (const auto &shoot: shoots)
	{
		dstrect.x = shoot.second.x_ * renderData_.rectSize_;
		dstrect.y = shoot.second.y_ * renderData_.rectSize_;
		fillrect.x = shoot.first.x_ * renderData_.rectSize_;
		fillrect.y = shoot.first.y_ * renderData_.rectSize_;
		if (shoot.first == shoot.second)
		{
			explosed_.add({shoot.first,renderData_.fps_});
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlExplosionTextures_, &explosion_rect, &dstrect);
		}
		else
		{
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &fillrect);
			SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlExplosionTextures_, &explosion_rect, &dstrect);
		}
	}

}

void Renderer::renderPlayerMove ()
{
	auto changed_positions = processor_->getChangedPositions();
	SDL_Rect dstrect;
	SDL_Rect prevrect;
	for (const auto &positions: changed_positions)
	{
		prevrect.x = positions.first.x_ * renderData_.rectSize_;
		prevrect.y = positions.first.y_ * renderData_.rectSize_;
		dstrect.x = positions.second.x_ * renderData_.rectSize_;
		dstrect.y = positions.second.y_ * renderData_.rectSize_;
		dstrect.w = dstrect.h = prevrect.w = prevrect.h = renderData_.rectSize_;
		switch (positions.second.direction_)
		{
			case Position::Direction::BOT:
			{
				if (SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_,
						&renderData_.playerRect_, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
				// FIXME не понимаю как я квадраты тут настраивал, надо отладиться получше с этими квадратами
			case Position::Direction::TOP:
			{
				// TODO вынести их в поля класса
				SDL_Rect top_player_rect = renderData_.playerRect_;
				top_player_rect.x += top_player_rect.w + top_player_rect.w + renderData_.rectSize_;
				top_player_rect.y += top_player_rect.h + top_player_rect.h;
				top_player_rect.w = static_cast<int> (top_player_rect.w * 1.1);
				if (SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlTankTopTextures_, &top_player_rect, &dstrect))
				{
					std::cout << "Can't render top direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::LEFT:
			{
				SDL_Rect left_player_rect = renderData_.playerRect_;
				left_player_rect.x += left_player_rect.w + left_player_rect.w + renderData_.rectSize_ +
						static_cast<int>(renderData_.rectSize_ * 1.5);
				left_player_rect.y += left_player_rect.h + left_player_rect.h - renderData_.rectSize_;
				if (SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlTankLeftTextures_, &left_player_rect, &dstrect))
				{
					std::cout << "Can't render left direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::RIGHT:
			{
				SDL_Rect right_player_rect = renderData_.playerRect_;
				right_player_rect.h = static_cast<int>(right_player_rect.h * 0.8);
				if (SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlTankRightTextures_, &right_player_rect, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::UNDEFINED:{
				throw std::logic_error("Move in unknow direction");
			}
		}
		SDL_RenderCopy(renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &prevrect);
	}
}

void Renderer::setProcessor (MainProcessor *processor)
{
	processor_ = processor;
}

void Renderer::prepare ()
{
	init();
	load();
	// Синхронизация с тем, чтобы быть уверенным, что все корректно прогрузилось перед отрисовкой.
	SDL_Delay(1);
}

Renderer::~Renderer ()
{
	quit();
}
