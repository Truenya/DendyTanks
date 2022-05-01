//
// Created by true on 2022-04-25.
//
#include "Renderer.h"
#include <SDL2/SDL_image.h>
#include <experimental/random>

int Renderer::loop()
{
//    static constexpr int speed = 10;
    if (init_()) {
        quit_();
        return 1;
    }

    if (load_()) {
        quit_();
        return 1;
    }
	while(run_.load())
		processEvents();
//    SDL_Rect r;
//
//    int x = 0;
//    int y = 0;
//
//    r.x = x;ggb
//
//    while (run_) {
//        processEvents(speed, x, y);
//        r.x = x;
//        r.y = y;

//        SDL_FillRect(scr, nullptr, SDL_MapRGB(scr -> format, 255, 255, 255));
//
//        SDL_BlitSurface(sdlWallTexture_, nullptr, scr, &r);
//
//        SDL_UpdateWindowSurface(sdlWindowTest_);

//        SDL_RenderClear(sdlRenderer_);
//        SDL_RenderCopy(sdlRenderer_, sdlWallTexture_, nullptr, nullptr   );
//        SDL_RenderPresent(sdlRenderer_);
//    }

    return quit_();
}

bool Renderer::init_() {
    bool initialized = false;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Can't init_: " << SDL_GetError() << std::endl;
        system("pause");
        initialized |= true;
    }
	dims = processor_->worldSize();

	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	screenWidth_ = DM.w-80;
	screenHeight_ = DM.h-80;



    sdlWindowTest_ = SDL_CreateWindow("Пробное окно SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      screenWidth_, screenHeight_, SDL_WINDOW_SHOWN );
    if (sdlWindowTest_ == nullptr) {
        std::cout << "Can't create window: " << SDL_GetError() << std::endl;
        initialized |= true;
    }

    sdlRenderer_ = SDL_CreateRenderer(sdlWindowTest_,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(sdlRenderer_ == nullptr)
    {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        initialized |= true;
    }
	if (SDL_RenderClear(sdlRenderer_))
	{

		std::cout << "Can't clear renderer: " << SDL_GetError() << std::endl;
		initialized |= true;
	}


	if (SDL_SetRenderDrawColor(sdlRenderer_, 0xFF, 0xFF, 0xFF, 0xFF))
	{
		std::cout << "Can't fill renderer: " << SDL_GetError() << std::endl;
		initialized |= true;
	}
	SDL_RenderClear(sdlRenderer_);

    int flags = IMG_INIT_PNG;
    if ( !( IMG_Init( flags ) & flags ) ) {
        std::cout << "Can't init_ image: " << IMG_GetError() << std::endl;
        initialized |= true;
    }

    scr = SDL_GetWindowSurface(sdlWindowTest_);

    return initialized;
}

bool Renderer::load_() {
    bool isLoadedIncorrectly = false;

    SDL_Surface * temp_surf = NULL;
    temp_surf = IMG_Load("cvetok.png");
    if (temp_surf == NULL) {
        std::cout << "Can't load image: " << IMG_GetError() << std::endl;
	    isLoadedIncorrectly |= true;
    }

	sdlWallTexture_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	SDL_FreeSurface(temp_surf);

	if(sdlWallTexture_ == nullptr){
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
		isLoadedIncorrectly |= true;
	}
	temp_surf = IMG_Load("tank.png");
	if (temp_surf == NULL) {
		std::cout << "Can't load image: " << IMG_GetError() << std::endl;
		isLoadedIncorrectly |= true;
	}

	sdlPlayerTexture_ = SDL_CreateTextureFromSurface(sdlRenderer_, temp_surf);
	if(sdlPlayerTexture_ == nullptr){
		std::cout << "Can't convert: " << SDL_GetError() << std::endl;
		isLoadedIncorrectly |= true;
	}
//    SDL_RenderClear(sdlRenderer_);
//	SDL_RenderDrawRects()
//    SDL_RenderCopy(sdlRenderer_, sdlWallTexture_, nullptr, nullptr   );
//    SDL_RenderPresent(sdlRenderer_);

	fillMap();


    return isLoadedIncorrectly;
}

int Renderer::quit_() {

    SDL_DestroyWindow(sdlWindowTest_);
    sdlWindowTest_ = nullptr;

    SDL_DestroyRenderer(sdlRenderer_);
    sdlRenderer_ = nullptr;

    SDL_DestroyTexture(sdlWallTexture_);

    SDL_Quit();
    IMG_Quit();
    return 0;
}

void Renderer::processEvents(){
    while(SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
	        run_.store(false);
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP) {
	            auto player = processor_->getPlayer();
				Position move = {0,-1 };
				auto com = new MoveCommandExample(player,move);
				processor_->addCommand(com);
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
	            auto com = new MoveCommandExample(processor_->getPlayer(),{0,1 });
	            processor_->addCommand(com);
            }
            if (e.key.keysym.sym == SDLK_RIGHT) {
	            auto com = new MoveCommandExample(processor_->getPlayer(),{1,0 });
	            processor_->addCommand(com);
            }
            if (e.key.keysym.sym == SDLK_LEFT) {
	            auto com = new MoveCommandExample(processor_->getPlayer(),{-1,0 });
	            processor_->addCommand(com);
            }
        }
    }
//	render();
}

Renderer::Renderer(std::atomic_bool &running):
	prevRender(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()),
	run_(running)
{}

void Renderer::fillMap() {
	rectSize =  (screenHeight_ / dims.y) - 1;
//	SDL_Point wallTextureSize;
//	SDL_QueryTexture(sdlWallTexture_, NULL, NULL, &wallTextureSize.x, &wallTextureSize.y);
	SDL_Point playerTextureSize;
	SDL_QueryTexture(sdlPlayerTexture_, NULL, NULL, &playerTextureSize.x, &playerTextureSize.y);
	playerRect.w = playerTextureSize.x / 3 - 1;
	playerRect.h = playerTextureSize.y / 3 - 1;

	for (int i = 0; i < dims.x ; ++i) {
		for (int j = 0; j < dims.y; ++j) {
			if( (processor_->at({i, j}).type != BaseGameObject::Type::Space)
				&&
				(processor_->at({i, j}).type != BaseGameObject::Type::Undefined)) {
				SDL_Rect dstrect;
				dstrect.x = i * rectSize;
				dstrect.y = j * rectSize;
				dstrect.w = dstrect.h = rectSize;
				if (processor_->at({i, j}).type == BaseGameObject::Type::Wall) {
					SDL_RenderCopy(sdlRenderer_, sdlWallTexture_, nullptr, &dstrect);
				} else if (processor_->at({i, j}).type == BaseGameObject::Type::Player)
				{
					SDL_RenderCopy(sdlRenderer_, sdlPlayerTexture_, &playerRect, &dstrect);
				}
			}
		}
	}
//	auto playerField = gameWorld_->at(0,0);
//	while (playerField->getType() != BaseGameObject::Type::Space)
//	{
//		int x = std::experimental::randint(0,dims.first);
//		int y = std::experimental::randint(0,dims.second);
//		playerField = gameWorld_->at(x,y);
//	}
//	playerField->setType(BaseGameObject::Type::player_);
	SDL_RenderPresent(sdlRenderer_);
}


bool Renderer::render() {
	// Замерим время выполнения
	long int curTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	// С прошлой отрисовки прошло сколько-то милисекунд
	const long int MS_DIF = curTimeMs - prevRender;
	auto dPositions = processor_->getChangedPositions();
	for (const auto &positions: dPositions) {
		SDL_Rect dstrect;
		dstrect.x = positions.second.x * rectSize;
		dstrect.y = positions.second.y * rectSize;
		dstrect.w = dstrect.h = rectSize;
		SDL_RenderCopy(sdlRenderer_, sdlPlayerTexture_, &playerRect, &dstrect);
	}
	// Чаще чем 60 раз в секунду рендерить не будем
	if (MS_DIF >= mspf) {
		// Если прошлая отрисовка не была проведена позади в будущем ;D
		if (MS_DIF > 0) {
			const long int milisecondsDelay = MS_DIF - mspf;
			if(milisecondsDelay>=0){
				updateFps(fpsChangeDirection::increment);
				if (milisecondsDelay<1000)
					SDL_Delay(milisecondsDelay);
			}
			else{
				updateFps(fpsChangeDirection::decrement);
			}
		}
		SDL_RenderPresent(sdlRenderer_);
		prevRender = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
	return true;
}

void Renderer::setProcessor(CommandsProcessor *processor) {
	processor_ = processor;
}

void Renderer::updateFps(Renderer::fpsChangeDirection direction) {
	if (fps < 30) {
		if (direction == fpsChangeDirection::increment)
			fps++;
		if (direction == fpsChangeDirection::decrement)
			fps--;
		mspf = MS_IN_SECOND / (fps > 0 ? fps : 1);
	}
}
