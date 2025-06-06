//
// Created by true on 2022-04-25.
//
#include "Renderer.h"
#include "Processors/Common/ResourceManager.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <thread>

void Renderer::processingEventsLoop ()
{
	while (work_.load ())
	{
		processEvents ();
		std::this_thread::sleep_for (std::chrono::milliseconds (1000 / 15));// 15 действий в секунду норм наверное
	}
}

bool Renderer::render ()
{
	typedef std::chrono::high_resolution_clock Clock;
	// Замерим время выполнения
	const long int CUR_TIME_MS = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now ().time_since_epoch ()).count ();
	rendered_ = fps_.makeSomePauseIfNeeded (CUR_TIME_MS);

	if (!rendered_) {
		renderData_.fps_ = fps_.fps_;
		return rendered_;
	}

	// Очищаем экран перед рендерингом
	SDL_SetRenderDrawColor (renderData_.sdlRenderer_, 0, 0, 0, 255);
	SDL_RenderClear (renderData_.sdlRenderer_);

	// Отрисовываем карту
	fillMap ();

	// Отрисовываем движущиеся объекты
	renderNpcMove ();
	renderPlayerMove ();
	renderShoots ();

	// Обновляем экран
	SDL_RenderPresent (renderData_.sdlRenderer_);

	renderData_.fps_ = fps_.fps_;
	return rendered_;
}

void Renderer::processEvents ()
{
	while (SDL_PollEvent (&renderData_.sdlEvent_))
	{
		if (renderData_.sdlEvent_.type == SDL_QUIT) {
			work_.store (false);
			return;
		}
		if (renderData_.sdlEvent_.type != SDL_KEYDOWN) continue;
		const auto PRESSED_KEY = renderData_.sdlEvent_.key.keysym.sym;
		Positions positions;
		switch (PRESSED_KEY)
		{
			case SDLK_UP: {
				positions.curPos_ = {0, -1, 0, Position::Direction::TOP};
				processor_->addCommand ({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid ()});
				break;
			}
			case SDLK_DOWN: {
				positions.curPos_ = {0, 1, 0, Position::Direction::BOT};
				processor_->addCommand ({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid ()});
				break;
			}
			case SDLK_RIGHT: {
				positions.curPos_ = {1, 0, 0, Position::Direction::RIGHT};
				processor_->addCommand ({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid ()});
				break;
			}
			case SDLK_LEFT: {
				positions.curPos_ = {-1, 0, 0, Position::Direction::LEFT};
				processor_->addCommand ({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid ()});
				break;
			}
			case SDLK_SPACE: {
				processor_->addCommand ({BaseCommand::Type::SHOOT_COMMAND, {}, GameWorld::my_uuid ()});
				break;
			}
			default:
				continue;
		}
	}
}

#ifndef MAKE_LOG
Renderer::Renderer (std::atomic_bool &running) :
    work_ (running),
    processor_ (nullptr)
{}
#else
Renderer::Renderer (std::atomic_bool &running, std::osyncstream &logs) :
    isCurrentlyWorking_ (running),
    processor_ (nullptr),
    logsSynchroStream_ (logs)
{
	renderData_.prevRender_ = duration_cast<milliseconds> (system_clock::now ().time_since_epoch ()).count_ ();
}
#endif

bool Renderer::init ()
{
	//    bool initialized = false;
	if (SDL_Init (SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Can't init: " << SDL_GetError () << std::endl;
		quit ();
	}
	renderData_.worldSize_ = processor_->worldSize ();

	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode (0, &dm);
	renderData_.screenWidth_ = dm.w - 80;
	renderData_.screenHeight_ = dm.h - 80;


	renderData_.sdlWindowTest_ = SDL_CreateWindow ("Пробное окно SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                                               static_cast<int> (renderData_.screenWidth_), static_cast<int> (renderData_.screenHeight_), SDL_WINDOW_SHOWN);
	if (renderData_.sdlWindowTest_ == nullptr)
	{
		std::cout << "Can't create window: " << SDL_GetError () << std::endl;
		quit ();
	}

	renderData_.sdlRenderer_ = SDL_CreateRenderer (renderData_.sdlWindowTest_, -1,
	                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderData_.sdlRenderer_ == nullptr)
	{
		std::cout << "Can't create renderer: " << SDL_GetError () << std::endl;
		quit ();
	}
	// Устанавливаем черный цвет фона
	if (SDL_SetRenderDrawColor (renderData_.sdlRenderer_, 0, 0, 0, 255))
	{
		std::cout << "Can't fill renderer: " << SDL_GetError () << std::endl;
		quit ();
	}

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init (flags) & flags))
	{
		std::cout << "Can't init image: " << IMG_GetError () << std::endl;
		quit ();
	}

	return true;
}

bool Renderer::load ()
{
	try {
		// Load wall texture
		renderData_.sdlWallTexture_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/cvetok.png");

		// Load fill texture
		renderData_.sdlFillTexture_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/fill.png");

		// Load tank textures
		renderData_.sdlTankBottomTextures_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/tank_b.png");
		renderData_.sdlTankTopTextures_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/tank_t.png");
		renderData_.sdlTankLeftTextures_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/tank_l.png");
		renderData_.sdlTankRightTextures_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/tank_r.png");

		// Load explosion texture
		renderData_.sdlExplosionTextures_ = ResourceManager::loadTexture (renderData_.sdlRenderer_, "../resources/explosion.png");

		// Fill the map with textures
		fillMap ();

		return true;
	}
	catch (const std::exception &e) {
		std::cerr << "Error loading textures: " << e.what () << std::endl;
		std::cerr << "Please make sure all required texture files exist in the resources directory." << std::endl;
		std::cerr << "Required textures: cvetok.png, fill.png, tank_b.png, tank_t.png, tank_l.png, tank_r.png, explosion.png" << std::endl;
		std::cerr << "Or their alternatives: tanks_b_green_blue_red_512x605.png, tanks_t_green_blue_red_512x605.png, etc." << std::endl;

		// Gracefully exit instead of segfaulting
		work_.store (false);
		return false;
	}
}

int Renderer::quit ()
{
	work_.store (false);
	SDL_DestroyRenderer (renderData_.sdlRenderer_);
	SDL_DestroyWindow (renderData_.sdlWindowTest_);
	SDL_DestroyTexture (renderData_.sdlWallTexture_);
	return 0;
}

void Renderer::fillMap ()
{
	static bool firstRun = true;

	// Подготавливаем текстуры только при первом запуске
	if (firstRun) {
		prepareTextures ();
		firstRun = false;
	}

	SDL_Rect dstrect;

	// Отрисовываем все клетки карты
	for (int i = 0; i < renderData_.worldSize_.x_; ++i)
	{
		for (int j = 0; j < renderData_.worldSize_.y_; ++j)
		{
			fillRectByPosition (dstrect, i, j);
		}
	}
}

void Renderer::fillRectByPosition (SDL_Rect &dstrect, int i, int j) const
{
	auto type = processor_->typeAt ({i, j});

	// Если тип неопределен, просто отрисовываем пустое пространство
	if (type == GameObject::Type::UNDEFINED) {
		setScreenPosition (dstrect, i, j);
		SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &dstrect);
		return;
	}

	setScreenPosition (dstrect, i, j);
	switch (type)
	{
		case GameObject::Type::WALL:
			if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlWallTexture_, nullptr, &dstrect))
				throw std::runtime_error ("Cannot render wall");
			break;
		case GameObject::Type::PLAYER:
			if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_, &renderData_.playerRect_, &dstrect))
				throw std::runtime_error ("Cannot render player");
			break;
		case GameObject::Type::ENEMY: {
			SDL_Rect enemy = renderData_.enemyRect_;
			enemy.x += enemy.w;
			enemy.y += enemy.h;
			if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_, &enemy, &dstrect))
				throw std::runtime_error ("Cannot render enemy");
			break;
		}
		case GameObject::Type::SPACE:
			if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &dstrect))
				throw std::runtime_error ("Cannot render space");
			break;
		default:
			// Для любых других типов отрисовываем пустое пространство
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &dstrect);
	}
}

void Renderer::setScreenPosition (SDL_Rect &dstrect, int i, int j) const
{
	dstrect.x = static_cast<int> (i * renderData_.rectSize_);
	dstrect.y = static_cast<int> (j * renderData_.rectSize_);
	dstrect.w = dstrect.h = static_cast<int> (renderData_.rectSize_);
}

void Renderer::prepareTextures ()
{
	// Очищаем экран перед началом рендеринга
	SDL_SetRenderDrawColor (renderData_.sdlRenderer_, 0, 0, 0, 255);
	SDL_RenderClear (renderData_.sdlRenderer_);

	// Вычисляем размер клетки на основе размера мира и экрана
	renderData_.rectSize_ = (renderData_.screenHeight_ / renderData_.worldSize_.y_) - 1;

	// Получаем размеры текстуры танка
	SDL_Point player_texture_size;
	if (SDL_QueryTexture (renderData_.sdlTankBottomTextures_, nullptr, nullptr, &player_texture_size.x, &player_texture_size.y))
		throw std::runtime_error ("cannot query texture");

	// Устанавливаем размеры прямоугольников для игрока и врага
	renderData_.playerRect_.w = player_texture_size.x / 3 - 1;
	renderData_.playerRect_.h = player_texture_size.y / 3 - 1;
	renderData_.enemyRect_.w = player_texture_size.x / 3 - 1;
	renderData_.enemyRect_.h = player_texture_size.y / 3 - 1;
}

void Renderer::renderShoots ()
{
	SDL_Rect fillrect;
	SDL_Rect dstrect;
	dstrect.w = dstrect.h = fillrect.w = fillrect.h = static_cast<int> (renderData_.rectSize_);
	// TODO некрасиво, поправить
	for (size_t i = 0; i < explosed_.count (); ++i)
	{
		if (!explosed_[i].second)
		{
			const Position pos = explosed_[i].first;
			fillrect.x = static_cast<int> (pos.x_ * renderData_.rectSize_);
			fillrect.y = static_cast<int> (pos.y_ * renderData_.rectSize_);
			if (processor_->noTankAtPos (pos))// FIXME не работает
				SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &fillrect);
			explosed_.remove (i);
		}
		else {
			if (rendered_)
				explosed_[i].second--;
		}
	}

	auto shoots = processor_->getShoots ();
	if (shoots.empty ()) return;
	SDL_Point explosion_texture_size;
	SDL_QueryTexture (renderData_.sdlExplosionTextures_, nullptr, nullptr, &explosion_texture_size.x, &explosion_texture_size.y);

	SDL_Rect explosion_rect;
	SDL_Rect projectile_rect;
	explosion_rect.w = projectile_rect.w = explosion_texture_size.x / 5;
	explosion_rect.h = projectile_rect.h = explosion_texture_size.y / 2;
	explosion_rect.x = explosion_rect.w * 4;
	explosion_rect.y = projectile_rect.y = projectile_rect.x = 0;


	for (const auto &shoot : shoots)
	{
		dstrect.x = static_cast<int> (shoot.second.x_ * renderData_.rectSize_);
		dstrect.y = static_cast<int> (shoot.second.y_ * renderData_.rectSize_);
		fillrect.x = static_cast<int> (shoot.first.x_ * renderData_.rectSize_);
		fillrect.y = static_cast<int> (shoot.first.y_ * renderData_.rectSize_);
		if (shoot.first == shoot.second)
		{
			explosed_.add ({shoot.first, renderData_.fps_});
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlExplosionTextures_, &explosion_rect, &dstrect);
		}
		else
		{
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &fillrect);
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlExplosionTextures_, &explosion_rect, &dstrect);
		}
	}
}

void Renderer::renderPlayerMove ()
{
	auto changed_positions = processor_->getPlayerChangedPositions ();
	SDL_Rect dstrect;
	SDL_Rect prevrect;
	for (const auto &positions : changed_positions)
	{
		prevrect.x = static_cast<int> (positions.first.x_ * renderData_.rectSize_);
		prevrect.y = static_cast<int> (positions.first.y_ * renderData_.rectSize_);
		dstrect.x = static_cast<int> (positions.second.x_ * renderData_.rectSize_);
		dstrect.y = static_cast<int> (positions.second.y_ * renderData_.rectSize_);
		dstrect.w = dstrect.h = prevrect.w = prevrect.h = static_cast<int> (renderData_.rectSize_);
		switch (positions.second.direction_)
		{
			case Position::Direction::BOT: {
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_,
				                    &renderData_.playerRect_, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			// FIXME не понимаю как я квадраты тут настраивал, надо отладиться получше с этими квадратами
			// Тут разные размеры полей танков еще.. жесть
			case Position::Direction::TOP: {
				// TODO вынести их в структурки
				SDL_Rect top_player_rect = renderData_.playerRect_;
				top_player_rect.x += top_player_rect.w * 2 + static_cast<int> (renderData_.rectSize_);
				top_player_rect.y += top_player_rect.h * 2;
				top_player_rect.w = static_cast<int> (top_player_rect.w * 1.1);
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankTopTextures_, &top_player_rect, &dstrect))
				{
					std::cout << "Can't render top direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::LEFT: {
				SDL_Rect left_player_rect = renderData_.playerRect_;
				left_player_rect.x += left_player_rect.w + left_player_rect.w + static_cast<int> (renderData_.rectSize_) +
				                      static_cast<int> (static_cast<double> (renderData_.rectSize_) * 1.5);
				left_player_rect.y += left_player_rect.h + left_player_rect.h - static_cast<int> (renderData_.rectSize_);
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankLeftTextures_, &left_player_rect, &dstrect))
				{
					std::cout << "Can't render left direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::RIGHT: {
				SDL_Rect right_player_rect = renderData_.playerRect_;
				right_player_rect.h = static_cast<int> (right_player_rect.h * 0.8);
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankRightTextures_, &right_player_rect, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::EQUAL: {
				std::cerr << "Move in unknown direction\n";
				//				throw std::logic_error("Move in unknown direction"); // FIXME возникает почему-то только когда танчики между собой играют без игрока
			}
		}
		if ((dstrect.x != prevrect.x) || (dstrect.y != prevrect.y))
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &prevrect);
	}
}

// TODO использовать текстурки с другим цветом
void Renderer::renderNpcMove ()
{
	auto changed_positions = processor_->getNpcChangedPositions ();
	SDL_Rect dstrect;
	SDL_Rect prevrect;
	for (const auto &positions : changed_positions)
	{
		prevrect.x = static_cast<int> (positions.first.x_ * renderData_.rectSize_);
		prevrect.y = static_cast<int> (positions.first.y_ * renderData_.rectSize_);
		dstrect.x = static_cast<int> (positions.second.x_ * renderData_.rectSize_);
		dstrect.y = static_cast<int> (positions.second.y_ * renderData_.rectSize_);
		dstrect.w = dstrect.h = prevrect.w = prevrect.h = static_cast<int> (renderData_.rectSize_);
		switch (positions.second.direction_)
		{
			case Position::Direction::BOT: {
				SDL_Rect enemy = renderData_.enemyRect_;
				enemy.x += enemy.w;
				enemy.y += enemy.h;
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankBottomTextures_,
				                    &enemy, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
				// FIXME не понимаю как я квадраты тут настраивал, надо отладиться получше с этими квадратами
				// Тут разные размеры полей танков еще.. жесть
			case Position::Direction::TOP: {
				// TODO вынести их в структурки
				SDL_Rect enemy = renderData_.enemyRect_;
				enemy.x += enemy.w * 2 + static_cast<int> (renderData_.rectSize_);
				enemy.y += enemy.h * 2;
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankTopTextures_, &enemy, &dstrect))
				{
					std::cout << "Can't render top direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::LEFT: {
				SDL_Rect enemy = renderData_.enemyRect_;
				enemy.x += enemy.w * 2 + static_cast<int> (renderData_.rectSize_);
				enemy.y += enemy.h;
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankLeftTextures_, &enemy, &dstrect))
				{
					std::cout << "Can't render left direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::RIGHT: {
				SDL_Rect enemy = renderData_.enemyRect_;
				enemy.h = static_cast<int> (enemy.h * 0.8);
				if (SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlTankRightTextures_, &enemy, &dstrect))
				{
					std::cout << "Can't render bottom direction: " << SDL_GetError () << std::endl;
				}
				break;
			}
			case Position::Direction::EQUAL: {
				std::cerr << "Move in unknown direction\n";
				//				throw std::logic_error("Move in unknown direction"); // FIXME возникает почему-то только когда танчики между собой играют без игрока
			}
		}
		if ((dstrect.x != prevrect.x) || (dstrect.y != prevrect.y))
			SDL_RenderCopy (renderData_.sdlRenderer_, renderData_.sdlFillTexture_, nullptr, &prevrect);
	}
}

void Renderer::setProcessor (MainProcessor *processor)
{
	processor_ = processor;
}

void Renderer::prepare ()
{
	try {
		if (!init ()) {
			std::cerr << "Failed to initialize renderer." << std::endl;
			work_.store (false);
			return;
		}

		if (!load ()) {
			std::cerr << "Failed to load resources." << std::endl;
			work_.store (false);
			return;
		}

		std::cout << "Renderer prepared successfully." << std::endl;
	}
	catch (const std::exception &e) {
		std::cerr << "Error preparing renderer: " << e.what () << std::endl;
		work_.store (false);
	}
}

Renderer::~Renderer ()
{
	quit ();
}
