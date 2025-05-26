#ifndef DENDY_TANKS_RENDER_SYSTEM_H
#define DENDY_TANKS_RENDER_SYSTEM_H

#include "../System.h"
#include "../Components/TransformComponent.h"
#include "../Components/RenderComponent.h"
#include "../../ResourceManager.h"
#include <SDL2/SDL.h>

/**
 * @class RenderSystem
 * @brief System responsible for rendering entities
 */
class RenderSystem : public System {
public:
    /**
     * @brief Constructor
     * @param renderer SDL renderer to use for rendering
     */
    explicit RenderSystem(SDL_Renderer* renderer) : renderer_(renderer) {
        // Add required components
        addRequiredComponent<TransformComponent>();
        addRequiredComponent<RenderComponent>();
    }
    
    /**
     * @brief Initialize the system
     */
    void initialize() override {
        // Nothing to initialize
    }
    
    /**
     * @brief Update the system (render entities)
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime) override {
        // Get all entities with required components
        auto entities = getCompatibleEntities();
        
        // Render each entity
        for (auto entity : entities) {
            auto transform = entity->getComponent<TransformComponent>();
            auto render = entity->getComponent<RenderComponent>();
            
            // Skip invisible entities
            if (!render->isVisible()) {
                continue;
            }
            
            // Get the texture
            SDL_Texture* texture = ResourceManager::getInstance().getTexture(render->getTextureId());
            if (!texture) {
                continue;
            }
            
            // Create destination rectangle
            SDL_Rect dstRect = {
                transform->getX(),
                transform->getY(),
                render->getWidth(),
                render->getHeight()
            };
            
            // Get source rectangle
            SDL_Rect srcRect = render->getSourceRect();
            
            // If source rectangle is not set, use the entire texture
            if (srcRect.w == 0 || srcRect.h == 0) {
                int textureWidth, textureHeight;
                SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
                srcRect = {0, 0, textureWidth, textureHeight};
            }
            
            // Calculate rotation angle based on direction
            double angle = 0.0;
            switch (transform->getDirection()) {
                case TransformComponent::Direction::UP:
                    angle = 0.0;
                    break;
                case TransformComponent::Direction::RIGHT:
                    angle = 90.0;
                    break;
                case TransformComponent::Direction::DOWN:
                    angle = 180.0;
                    break;
                case TransformComponent::Direction::LEFT:
                    angle = 270.0;
                    break;
                default:
                    break;
            }
            
            // Render the entity
            SDL_RenderCopyEx(renderer_, texture, &srcRect, &dstRect, angle, nullptr, SDL_FLIP_NONE);
        }
    }
    
    /**
     * @brief Get the system name
     * @return Name of the system
     */
    std::string getName() const override {
        return "RenderSystem";
    }

private:
    SDL_Renderer* renderer_;
};

#endif // DENDY_TANKS_RENDER_SYSTEM_H