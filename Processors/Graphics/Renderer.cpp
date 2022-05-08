//
// Created by true on 2022-04-25.
//
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include "../../Entities/Commands/MoveCommand.h"
#include "../../Entities/Commands/ShootCommand.h"

void Renderer::processingEventsLoop ()
{
//	init();
//	load();
	while (work_.load())
	{
		processEvents();
//		render();
	}
	quit();
	return ;
}

bool Renderer::init ()
{
//    bool initialized = false;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Can't init: " << SDL_GetError() << std::endl;
		quit();
	}
	worldSize_ = processor_->worldSize();

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	screenWidth_ = dm.w - 80;
	screenHeight_ = dm.h - 80;


	sdlWindowTest_ = SDL_CreateWindow("Пробное окно SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			screenWidth_, screenHeight_, SDL_WINDOW_SHOWN);
	if (sdlWindowTest_ == nullptr)
	{
		std::cout << "Can't create window: " << SDL_GetError() << std::endl;
		quit();
	}

	sdlRenderer_ = SDL_CreateRenderer(sdlWindowTest_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRenderer_ == nullptr)
	{
		std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
		quit();
	}

	if (SDL_SetRenderDrawColor(sdlRenderer_, 0xFF, 0xFF, 0xFF, 0xFF))
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
	sdlWallTexture_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	SDL_FreeSurface(temp_surf);

	if (sdlWallTexture_ == nullptr)
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
	sdlFillTexture_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	SDL_FreeSurface(temp_surf);

	if (sdlFillTexture_ == nullptr)
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
	sdlTankBottomTextures_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if (sdlTankBottomTextures_ == nullptr)
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
	sdlTankTopTextures_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if (sdlTankTopTextures_ == nullptr)
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
	sdlTankLeftTextures_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if (sdlTankLeftTextures_ == nullptr)
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
	sdlTankRightTextures_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if (sdlTankRightTextures_ == nullptr)
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
	sdlExplosionTextures_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if (sdlExplosionTextures_ == nullptr)
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
	SDL_DestroyWindow(sdlWindowTest_);
	sdlWindowTest_ = nullptr;

	SDL_DestroyRenderer(sdlRenderer_);
	sdlRenderer_ = nullptr;

	SDL_DestroyTexture(sdlWallTexture_);

	SDL_Quit();
	IMG_Quit();
	return 0;
}

void Renderer::processEvents ()
{
	while (SDL_PollEvent(&sdlEvent_))
	{
		if (sdlEvent_.type == SDL_QUIT)	work_.store(false);
		if (sdlEvent_.type != SDL_KEYDOWN)	continue;
		const auto PRESSED_KEY = sdlEvent_.key.keysym.sym;
		switch (PRESSED_KEY)
		{
			case SDLK_UP: processor_->addPlayerCommand(new MoveCommand(processor_->getPlayer(),
					{0, -1, 0, Position::Direction::TOP})); break;
			case SDLK_DOWN: processor_->addPlayerCommand(new MoveCommand(processor_->getPlayer(),
					{0, 1, 0, Position::Direction::BOT})); break;
			case SDLK_RIGHT: processor_->addPlayerCommand(new MoveCommand(processor_->getPlayer(),
					{1, 0, 0, Position::Direction::RIGHT})); break;
			case SDLK_LEFT: processor_->addPlayerCommand(new MoveCommand(processor_->getPlayer(),
					{-1, 0, 0, Position::Direction::LEFT})); break;
			case SDLK_SPACE: processor_->addPlayerCommand(new ShootCommand(processor_->getPlayer(),
					BaseCommand::Type::SHOOT_COMMAND)); break;
			default:
				continue;
		}
	}
}

Renderer::Renderer (std::atomic_bool &running) :
		prevRender_(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()),
		work_(running)
{}

void Renderer::fillMap ()
{
	prepareTextures();

	SDL_Rect dstrect;
	for (int i = 0; i < worldSize_.x_; ++i)
	{
		for (int j = 0; j < worldSize_.y_; ++j)
		{
			fillRectByPosition(dstrect, i, j);
		}
	}
	SDL_RenderPresent(sdlRenderer_);
}

void Renderer::fillRectByPosition (SDL_Rect &dstrect, int i, int j) const
{
	auto type = processor_->at({i, j}).type_;
	if (type == BaseGameObject::Type::UNDEFINED)
		throw std::logic_error ("Trying to render unknown object");
	setScreenPosition(dstrect, i, j);
	switch (type)
	{
		case BaseGameObject::Type::WALL:
			SDL_RenderCopy(sdlRenderer_, sdlWallTexture_, nullptr, &dstrect);
			break;
		case BaseGameObject::Type::PLAYER:
			SDL_RenderCopy(sdlRenderer_, sdlTankBottomTextures_, &playerRect_, &dstrect);
			break;
		case BaseGameObject::Type::SPACE:
			SDL_RenderCopy(sdlRenderer_, sdlFillTexture_, nullptr, &dstrect);
			break;
		default:
			throw std::logic_error ("Trying to render unknown object");
	}
}

void Renderer::setScreenPosition (SDL_Rect &dstrect, int i, int j) const
{
	dstrect.x = i * rectSize_;
	dstrect.y = j * rectSize_;
	dstrect.w = dstrect.h = rectSize_;
}

void Renderer::prepareTextures ()
{
	SDL_RenderCopy(sdlRenderer_, sdlFillTexture_, nullptr, nullptr);
	rectSize_ = (screenHeight_ / worldSize_.y_) - 1;
	SDL_Point player_texture_size;
	SDL_QueryTexture(sdlTankBottomTextures_, NULL, NULL, &player_texture_size.x, &player_texture_size.y);
	playerRect_.w = player_texture_size.x / 3 - 1;
	playerRect_.h = player_texture_size.y / 3 - 1;
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
	const long int MS_DIF = cur_time_ms - prevRender_;
	// Чаще чем 30 раз в секунду рендерить не будем
	if (MS_DIF >= mspf_)
	{
		// Если прошлая отрисовка не была проведена позади в будущем ;D
		if (MS_DIF > 0)
		{
			const long int MILISECONDS_DELAY = MS_DIF - mspf_;
			renderTime_.store(MS_DIF);
			if (MILISECONDS_DELAY >= 0)
			{
				updateFps(FpsChangeDirection::INCREMENT);
				if (MILISECONDS_DELAY < 1000)
				{
					SDL_Delay(MILISECONDS_DELAY);
					return false;
				}
			}
			else
			{
				updateFps(FpsChangeDirection::DECREMENT);
			}
		}
		SDL_RenderPresent(sdlRenderer_);
		prevRender_ = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
	return true;
}

void Renderer::renderPlayerShoots ()
{
	auto shoots = processor_->getShoots();
	SDL_Point explosion_texture_size;
	SDL_QueryTexture(sdlExplosionTextures_, NULL, NULL, &explosion_texture_size.x, &explosion_texture_size.y);

	SDL_Rect explosion_rect;
	SDL_Rect projectile_rect;
	explosion_rect.w = projectile_rect.w = explosion_texture_size.x / 5;
	explosion_rect.h = projectile_rect.h = explosion_texture_size.y / 2;
	explosion_rect.x = explosion_rect.w * 4;
	explosion_rect.y = projectile_rect.y = projectile_rect.x = 0;


	SDL_Rect dstrect;
	SDL_Rect fillrect;
	dstrect.w = dstrect.h = fillrect.w = fillrect.h = rectSize_;
	for (const auto &shoot: shoots)
	{
		dstrect.x = shoot.second.x_ * rectSize_;
		dstrect.y = shoot.second.y_ * rectSize_;
		fillrect.x = shoot.first.x_ * rectSize_;
		fillrect.y = shoot.first.y_ * rectSize_;
		if (shoot.first == shoot.second)
		{
			SDL_RenderCopy(sdlRenderer_, sdlExplosionTextures_, &explosion_rect, &dstrect);
		}
		else
		{
			SDL_RenderCopy(sdlRenderer_, sdlFillTexture_, nullptr, &fillrect);
			SDL_RenderCopy(sdlRenderer_, sdlExplosionTextures_, &explosion_rect, &dstrect);
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
		prevrect.x = positions.first.x_ * rectSize_;
		prevrect.y = positions.first.y_ * rectSize_;
		dstrect.x = positions.second.x_ * rectSize_;
		dstrect.y = positions.second.y_ * rectSize_;
		dstrect.w = dstrect.h = prevrect.w = prevrect.h = rectSize_;
		switch (positions.second.mDirection_)
		{
			case Position::Direction::BOT:
			{
				if (SDL_RenderCopy(sdlRenderer_, sdlTankBottomTextures_, &playerRect_, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
				// FIXME не понимаю как я квадраты тут настраивал, надо отладиться получше с этими квадратами
			case Position::Direction::TOP:
			{
				// TODO вынести их в поля класса
				SDL_Rect top_player_rect = playerRect_;
				top_player_rect.x += top_player_rect.w + top_player_rect.w + rectSize_;
				top_player_rect.y += top_player_rect.h + top_player_rect.h;
				top_player_rect.w *= 1.1;
				if (SDL_RenderCopy(sdlRenderer_, sdlTankTopTextures_, &top_player_rect, &dstrect))
				{
					std::cout << "Can't render top direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::LEFT:
			{
				SDL_Rect left_player_rect = playerRect_;
				left_player_rect.x += left_player_rect.w + left_player_rect.w + rectSize_ + static_cast<int>(rectSize_ * 1.5);
				left_player_rect.y += left_player_rect.h + left_player_rect.h - rectSize_;
				if (SDL_RenderCopy(sdlRenderer_, sdlTankLeftTextures_, &left_player_rect, &dstrect))
				{
					std::cout << "Can't render left direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::RIGHT:
			{
				SDL_Rect right_player_rect = playerRect_;
				right_player_rect.h = static_cast<int>(right_player_rect.h * 0.8);
				if (SDL_RenderCopy(sdlRenderer_, sdlTankRightTextures_, &right_player_rect, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
				}
				break;
			}
			case Position::Direction::UNDEFINED:{
				throw std::logic_error("Move in unknow direction");
			}
		}
		SDL_RenderCopy(sdlRenderer_, sdlFillTexture_, nullptr, &prevrect);
	}
}

void Renderer::setProcessor (CommandsProcessor *processor)
{
	processor_ = processor;
}

void Renderer::updateFps (Renderer::FpsChangeDirection direction)
{
	if (fps_ < 30)
	{
		if (direction == FpsChangeDirection::INCREMENT)
		{
			fps_++;
		}
		if (direction == FpsChangeDirection::DECREMENT)
		{
			fps_--;
		}
		mspf_ = MS_IN_SECOND / (fps_ > 0 ? fps_ : 1);
	}
}

void Renderer::prepare ()
{
	init();
	load();
}