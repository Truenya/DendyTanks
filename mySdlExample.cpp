//
// Created by true on 2022-04-25.
//
#include "mySdlExample.h"
#include <SDL2/SDL_image.h>
#include <experimental/random>

int mySdlExample::loop()
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
	while(run)
		processEvents(rectSize);
//    SDL_Rect r;
//
//    int x = 0;
//    int y = 0;
//
//    r.x = x;ggb
//
//    while (run) {
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

bool mySdlExample::init_() {
    bool initialized = false;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Can't init_: " << SDL_GetError() << std::endl;
        system("pause");
        initialized |= true;
    }
	gameWorld_.reset(WorldGenerator::generateWorld("labirinth.txt"));
	dims = gameWorld_->size();

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

bool mySdlExample::load_() {
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

int mySdlExample::quit_() {

    SDL_DestroyWindow(sdlWindowTest_);
    sdlWindowTest_ = nullptr;

    SDL_DestroyRenderer(sdlRenderer_);
    sdlRenderer_ = nullptr;

    SDL_DestroyTexture(sdlWallTexture_);

    SDL_Quit();
    IMG_Quit();
    return 0;
}

void mySdlExample::processEvents(const int speed){//, int &x, int &y) {
	int x,y;
	x=y=0;
    while(SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            run = false;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_UP) {
                y -= speed;
            }
            if (e.key.keysym.sym == SDLK_DOWN) {
                y += speed;
            }
            if (e.key.keysym.sym == SDLK_RIGHT) {
                x += speed;
            }
            if (e.key.keysym.sym == SDLK_LEFT) {
                x -= speed;
            }
        }
    }
	long int cur_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	long int dif = cur_time-prevRender;
	if (dif >= 1000/60) {
		if (dif<1000 && dif > 0) {
			long int milisecondsPerFrame = (1000 - dif) / 60;
			SDL_Delay(milisecondsPerFrame);
			SDL_RenderPresent(sdlRenderer_);
		}
		prevRender = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
	sched_yield();
}

mySdlExample::mySdlExample():
	prevRender(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count())
{}

void mySdlExample::fillMap() {
	rectSize =  (screenHeight_ / dims.second) - 1;
//	SDL_Point wallTextureSize;
//	SDL_QueryTexture(sdlWallTexture_, NULL, NULL, &wallTextureSize.x, &wallTextureSize.y);
	SDL_Point playerTextureSize;
	SDL_QueryTexture(sdlPlayerTexture_, NULL, NULL, &playerTextureSize.x, &playerTextureSize.y);

	for (int i = 0; i < dims.first ; ++i) {
		for (int j = 0; j < dims.second; ++j) {
			if( (gameWorld_->at(i, j)->getType() != GameObjectExample::Type::Space)
				&&
				(gameWorld_->at(i, j)->getType() != GameObjectExample::Type::Undefined)) {
				SDL_Rect srcrect;
				srcrect.x = 0;
				srcrect.y = 0;
				srcrect.w = rectSize;
				srcrect.h = rectSize;
				SDL_Rect dstrect;
				dstrect.x = i * rectSize;
				dstrect.y = j * rectSize;
				dstrect.w = dstrect.h = rectSize;
				if (gameWorld_->at(i, j)->getType() == GameObjectExample::Type::Wall) {
					SDL_RenderCopy(sdlRenderer_, sdlWallTexture_, nullptr, &dstrect);
				} else if (gameWorld_->at(i, j)->getType() == GameObjectExample::Type::Player)
				{
					srcrect.w = playerTextureSize.x/3 - 1;
					srcrect.h = playerTextureSize.y/3 - 1;
					SDL_RenderCopy(sdlRenderer_, sdlPlayerTexture_, &srcrect, &dstrect);
				}
			}
		}
	}
//	auto playerField = gameWorld_->at(0,0);
//	while (playerField->getType() != GameObjectExample::Type::Space)
//	{
//		int x = std::experimental::randint(0,dims.first);
//		int y = std::experimental::randint(0,dims.second);
//		playerField = gameWorld_->at(x,y);
//	}
//	playerField->setType(GameObjectExample::Type::Player);
	SDL_RenderPresent(sdlRenderer_);
}
