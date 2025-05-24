#include "SDL2Renderer.h"
#include <SDL2/SDL_image.h>
#include <thread>

#ifndef MAKE_LOG
SDL2Renderer::SDL2Renderer() : BaseRenderer()
{
    isRunning = true;
}
#else
SDL2Renderer::SDL2Renderer(std::osyncstream& logStream) : 
    BaseRenderer(),
    logsSynchroStream(logStream)
{
    isRunning = true;
}
#endif

SDL2Renderer::~SDL2Renderer()
{
    SDL_DestroyRenderer(renderData.sdlRenderer_);
    SDL_DestroyWindow(renderData.sdlWindowTest_);
    SDL_DestroyTexture(renderData.sdlWallTexture_);
}

bool SDL2Renderer::prepare()
{
    try {
        if (!init()) {
            std::cerr << "Failed to initialize renderer." << std::endl;
            isRunning = false;
            return false;
        }
        
        if (!load()) {
            std::cerr << "Failed to load resources." << std::endl;
            isRunning = false;
            return false;
        }
        
        std::cout << "Renderer prepared successfully." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error preparing renderer: " << e.what() << std::endl;
        isRunning = false;
        return false;
    }
}

void SDL2Renderer::processingEventsLoop()
{
    while (isRunning.load())
    {
        processEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/15)); // 15 действий в секунду
    }
}

bool SDL2Renderer::render()
{
    typedef std::chrono::high_resolution_clock Clock;
    // Замерим время выполнения
    const long int CUR_TIME_MS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    rendered = fpsController.makeSomePauseIfNeeded(CUR_TIME_MS);
    
    if (rendered) {
        // Очищаем экран перед рендерингом
        SDL_SetRenderDrawColor(renderData.sdlRenderer_, 0, 0, 0, 255);
        SDL_RenderClear(renderData.sdlRenderer_);
        
        // Отрисовываем карту
        fillMap();
        
        // Отрисовываем движущиеся объекты
        renderNpcMove();
        renderPlayerMove();
        renderShoots();
        
        // Обновляем экран
        SDL_RenderPresent(renderData.sdlRenderer_);
    }
    
    // Обновляем FPS
    renderData.fps_ = fpsController.fps_;
    updateFps(fpsController.fps_);
    
    return rendered;
}

void SDL2Renderer::processEvents()
{
    processSDLEvents();
}

void SDL2Renderer::processSDLEvents()
{
    while (SDL_PollEvent(&renderData.sdlEvent_))
    {
        if (renderData.sdlEvent_.type == SDL_QUIT) { 
            isRunning = false;  
            return;
        }
        if (renderData.sdlEvent_.type != SDL_KEYDOWN)    
            continue;
        
        const auto PRESSED_KEY = renderData.sdlEvent_.key.keysym.sym;
        Positions positions;
        switch (PRESSED_KEY)
        {
            case SDLK_UP:
            {
                positions.curPos_ = {0, -1, 0, Position::Direction::TOP};
                processor->addCommand({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid()});
                break;
            }
            case SDLK_DOWN:
            {
                positions.curPos_ = {0, 1, 0, Position::Direction::BOT};
                processor->addCommand({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid()});
                break;
            }
            case SDLK_RIGHT:
            {
                positions.curPos_ = {1, 0, 0, Position::Direction::RIGHT};
                processor->addCommand({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid()});
                break;
            }
            case SDLK_LEFT:
            {
                positions.curPos_ = {-1, 0, 0, Position::Direction::LEFT};
                processor->addCommand({BaseCommand::Type::MOVE_COMMAND, positions, GameWorld::my_uuid()});
                break;
            }
            case SDLK_SPACE:
            {
                processor->addCommand({BaseCommand::Type::SHOOT_COMMAND, {}, GameWorld::my_uuid()});
                break;
            }
            default:
                continue;
        }
    }
}

bool SDL2Renderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Can't init: " << SDL_GetError() << std::endl;
        return false;
    }
    renderData.worldSize_ = processor->worldSize();

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    renderData.screenWidth_ = dm.w - 80;
    renderData.screenHeight_ = dm.h - 80;

    renderData.sdlWindowTest_ = SDL_CreateWindow("DendyTanks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            static_cast<int>(renderData.screenWidth_), static_cast<int>(renderData.screenHeight_), SDL_WINDOW_SHOWN);
    if (renderData.sdlWindowTest_ == nullptr)
    {
        std::cout << "Can't create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderData.sdlRenderer_ = SDL_CreateRenderer(renderData.sdlWindowTest_, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderData.sdlRenderer_ == nullptr)
    {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Устанавливаем черный цвет фона
    if (SDL_SetRenderDrawColor(renderData.sdlRenderer_, 0, 0, 0, 255))
    {
        std::cout << "Can't fill renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags) & flags))
    {
        std::cout << "Can't init image: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

bool SDL2Renderer::load()
{
    try {
        // Load wall texture
        renderData.sdlWallTexture_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/cvetok.png");
        
        // Load fill texture
        renderData.sdlFillTexture_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/fill.png");
        
        // Load tank textures
        renderData.sdlTankBottomTextures_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/tank_b.png");
        renderData.sdlTankTopTextures_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/tank_t.png");
        renderData.sdlTankLeftTextures_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/tank_l.png");
        renderData.sdlTankRightTextures_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/tank_r.png");
        
        // Load explosion texture
        renderData.sdlExplosionTextures_ = ResourceManager::loadTexture(renderData.sdlRenderer_, "../resources/explosion.png");
        
        // Fill the map with textures
        fillMap();
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading textures: " << e.what() << std::endl;
        std::cerr << "Please make sure all required texture files exist in the resources directory." << std::endl;
        std::cerr << "Required textures: cvetok.png, fill.png, tank_b.png, tank_t.png, tank_l.png, tank_r.png, explosion.png" << std::endl;
        std::cerr << "Or their alternatives: tanks_b_green_blue_red_512x605.png, tanks_t_green_blue_red_512x605.png, etc." << std::endl;
        
        // Gracefully exit instead of segfaulting
        isRunning = false;
        return false;
    }
}

void SDL2Renderer::fillMap()
{
    static bool firstRun = true;
    
    // Подготавливаем текстуры только при первом запуске
    if (firstRun) {
        prepareTextures();
        firstRun = false;
    }

    SDL_Rect dstrect;
    
    // Отрисовываем все клетки карты
    for (int i = 0; i < renderData.worldSize_.x_; ++i)
    {
        for (int j = 0; j < renderData.worldSize_.y_; ++j)
        {
            fillRectByPosition(dstrect, i, j);
        }
    }
}

void SDL2Renderer::fillRectByPosition(SDL_Rect &dstrect, int i, int j) const
{
    auto type = processor->typeAt({i, j});
    
    // Если тип неопределен, просто отрисовываем пустое пространство
    if (type == GameObject::Type::UNDEFINED) {
        setScreenPosition(dstrect, i, j);
        SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &dstrect);
        return;
    }
    
    setScreenPosition(dstrect, i, j);
    switch (type)
    {
        case GameObject::Type::WALL:
            if(SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlWallTexture_, nullptr, &dstrect))
                throw std::runtime_error("Cannot render wall");
            break;
        case GameObject::Type::PLAYER:
            if(SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankBottomTextures_, &renderData.playerRect_, &dstrect))
                throw std::runtime_error("Cannot render player");
            break;
        case GameObject::Type::ENEMY:
        {
            SDL_Rect enemy = renderData.enemyRect_;
            enemy.x += enemy.w;
            enemy.y += enemy.h;
            if(SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankBottomTextures_, &enemy, &dstrect))
                throw std::runtime_error("Cannot render enemy");
            break;
        }
        case GameObject::Type::SPACE:
            if(SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &dstrect))
                throw std::runtime_error("Cannot render space");
            break;
        default:
            // Для любых других типов отрисовываем пустое пространство
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &dstrect);
    }
}

void SDL2Renderer::setScreenPosition(SDL_Rect &dstrect, int i, int j) const
{
    dstrect.x = static_cast<int>(i * renderData.rectSize_);
    dstrect.y = static_cast<int>(j * renderData.rectSize_);
    dstrect.w = dstrect.h = static_cast<int>(renderData.rectSize_);
}

void SDL2Renderer::prepareTextures()
{
    // Очищаем экран перед началом рендеринга
    SDL_SetRenderDrawColor(renderData.sdlRenderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderData.sdlRenderer_);
    
    // Вычисляем размер клетки на основе размера мира и экрана
    renderData.rectSize_ = (renderData.screenHeight_ / renderData.worldSize_.y_) - 1;
    
    // Получаем размеры текстуры танка
    SDL_Point player_texture_size;
    if(SDL_QueryTexture(renderData.sdlTankBottomTextures_, nullptr, nullptr, &player_texture_size.x, &player_texture_size.y))
        throw std::runtime_error("cannot query texture");
    
    // Устанавливаем размеры прямоугольников для игрока и врага
    renderData.playerRect_.w = player_texture_size.x / 3 - 1;
    renderData.playerRect_.h = player_texture_size.y / 3 - 1;
    renderData.enemyRect_.w = player_texture_size.x / 3 - 1;
    renderData.enemyRect_.h = player_texture_size.y / 3 - 1;
}

void SDL2Renderer::renderShoots()
{
    SDL_Rect fillrect;
    SDL_Rect dstrect;
    dstrect.w = dstrect.h = fillrect.w = fillrect.h = static_cast<int>(renderData.rectSize_);
    
    // Обработка взрывов
    for (size_t i = 0; i < explosed.count(); ++i)
    {
        if (!explosed[i].second)
        {
            const Position pos = explosed[i].first;
            fillrect.x = static_cast<int>(pos.x_ * renderData.rectSize_);
            fillrect.y = static_cast<int>(pos.y_ * renderData.rectSize_);
            if (processor->noTankAtPos(pos))
                SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &fillrect);
            explosed.remove(i);
        }
        else {
            if (rendered)
                explosed[i].second--;
        }
    }
    
    // Обработка выстрелов
    auto shoots = processor->getShoots();
    if(shoots.empty()) return;
    
    SDL_Point explosion_texture_size;
    SDL_QueryTexture(renderData.sdlExplosionTextures_, nullptr, nullptr, &explosion_texture_size.x, &explosion_texture_size.y);

    SDL_Rect explosion_rect;
    SDL_Rect projectile_rect;
    explosion_rect.w = projectile_rect.w = explosion_texture_size.x / 5;
    explosion_rect.h = projectile_rect.h = explosion_texture_size.y / 2;
    explosion_rect.x = explosion_rect.w * 4;
    explosion_rect.y = projectile_rect.y = projectile_rect.x = 0;

    for (const auto &shoot: shoots)
    {
        dstrect.x = static_cast<int>(shoot.second.x_ * renderData.rectSize_);
        dstrect.y = static_cast<int>(shoot.second.y_ * renderData.rectSize_);
        fillrect.x = static_cast<int>(shoot.first.x_ * renderData.rectSize_);
        fillrect.y = static_cast<int>(shoot.first.y_ * renderData.rectSize_);
        if (shoot.first == shoot.second)
        {
            explosed.add({shoot.first, renderData.fps_});
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlExplosionTextures_, &explosion_rect, &dstrect);
        }
        else
        {
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &fillrect);
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlExplosionTextures_, &explosion_rect, &dstrect);
        }
    }
}

void SDL2Renderer::renderPlayerMove()
{
    auto changed_positions = processor->getPlayerChangedPositions();
    SDL_Rect dstrect;
    SDL_Rect prevrect;
    for (const auto &positions: changed_positions)
    {
        prevrect.x = static_cast<int>(positions.first.x_ * renderData.rectSize_);
        prevrect.y = static_cast<int>(positions.first.y_ * renderData.rectSize_);
        dstrect.x = static_cast<int>(positions.second.x_ * renderData.rectSize_);
        dstrect.y = static_cast<int>(positions.second.y_ * renderData.rectSize_);
        dstrect.w = dstrect.h = prevrect.w = prevrect.h = static_cast<int>(renderData.rectSize_);
        switch (positions.second.direction_)
        {
            case Position::Direction::BOT:
            {
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankBottomTextures_,
                        &renderData.playerRect_, &dstrect))
                {
                    std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::TOP:
            {
                SDL_Rect top_player_rect = renderData.playerRect_;
                top_player_rect.x += top_player_rect.w * 2 + static_cast<int>(renderData.rectSize_);
                top_player_rect.y += top_player_rect.h * 2;
                top_player_rect.w = static_cast<int>(top_player_rect.w * 1.1);
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankTopTextures_, &top_player_rect, &dstrect))
                {
                    std::cout << "Can't render top direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::LEFT:
            {
                SDL_Rect left_player_rect = renderData.playerRect_;
                left_player_rect.x += left_player_rect.w + left_player_rect.w + static_cast<int>(renderData.rectSize_) +
                        static_cast<int>(static_cast<double>(renderData.rectSize_) * 1.5);
                left_player_rect.y += left_player_rect.h + left_player_rect.h - static_cast<int>(renderData.rectSize_);
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankLeftTextures_, &left_player_rect, &dstrect))
                {
                    std::cout << "Can't render left direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::RIGHT:
            {
                SDL_Rect right_player_rect = renderData.playerRect_;
                right_player_rect.h = static_cast<int>(right_player_rect.h * 0.8);
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankRightTextures_, &right_player_rect, &dstrect))
                {
                    std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::EQUAL:{
                std::cerr << "Move in unknown direction\n";
            }
        }
        if ((dstrect.x != prevrect.x) || (dstrect.y != prevrect.y))
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &prevrect);
    }
}

void SDL2Renderer::renderNpcMove()
{
    auto changed_positions = processor->getNpcChangedPositions();
    SDL_Rect dstrect;
    SDL_Rect prevrect;
    for (const auto &positions: changed_positions)
    {
        prevrect.x = static_cast<int>(positions.first.x_ * renderData.rectSize_);
        prevrect.y = static_cast<int>(positions.first.y_ * renderData.rectSize_);
        dstrect.x = static_cast<int>(positions.second.x_ * renderData.rectSize_);
        dstrect.y = static_cast<int>(positions.second.y_ * renderData.rectSize_);
        dstrect.w = dstrect.h = prevrect.w = prevrect.h = static_cast<int>(renderData.rectSize_);
        switch (positions.second.direction_)
        {
            case Position::Direction::BOT:
            {
                SDL_Rect enemy = renderData.enemyRect_;
                enemy.x += enemy.w;
                enemy.y += enemy.h;
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankBottomTextures_,
                        &enemy, &dstrect))
                {
                    std::cout << "Can't render bottom direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::TOP:
            {
                SDL_Rect enemy = renderData.enemyRect_;
                enemy.x += enemy.w * 2 + static_cast<int>(renderData.rectSize_);
                enemy.y += enemy.h * 2;
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankTopTextures_, &enemy, &dstrect))
                {
                    std::cout << "Can't render top direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::LEFT:
            {
                SDL_Rect enemy = renderData.enemyRect_;
                enemy.x += enemy.w * 2 + static_cast<int>(renderData.rectSize_);
                enemy.y += enemy.h;
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankLeftTextures_, &enemy, &dstrect))
                {
                    std::cout << "Can't render left direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::RIGHT:
            {
                SDL_Rect enemy = renderData.enemyRect_;
                enemy.h = static_cast<int>(enemy.h * 0.8);
                if (SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlTankRightTextures_, &enemy, &dstrect))
                {
                    std::cout << "Can't render right direction: " << SDL_GetError() << std::endl;
                }
                break;
            }
            case Position::Direction::EQUAL:{
                std::cerr << "Move in unknown direction\n";
            }
        }
        if ((dstrect.x != prevrect.x) || (dstrect.y != prevrect.y))
            SDL_RenderCopy(renderData.sdlRenderer_, renderData.sdlFillTexture_, nullptr, &prevrect);
    }
}