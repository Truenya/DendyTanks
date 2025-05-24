#ifndef DENDY_TANKS_COLLISION_SYSTEM_H
#define DENDY_TANKS_COLLISION_SYSTEM_H

#include "../System.h"
#include "../Components/TransformComponent.h"
#include "../Components/CollisionComponent.h"
#include <SDL2/SDL.h>

/**
 * @class CollisionSystem
 * @brief System responsible for detecting and handling collisions between entities
 */
class CollisionSystem : public System {
public:
    /**
     * @brief Constructor
     */
    CollisionSystem() {
        // Add required components
        addRequiredComponent<TransformComponent>();
        addRequiredComponent<CollisionComponent>();
    }
    
    /**
     * @brief Initialize the system
     */
    void initialize() override {
        // Nothing to initialize
    }
    
    /**
     * @brief Update the system (check for collisions)
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime) override {
        // Get all entities with required components
        auto entities = getCompatibleEntities();
        
        // Check for collisions between all pairs of entities
        for (size_t i = 0; i < entities.size(); ++i) {
            for (size_t j = i + 1; j < entities.size(); ++j) {
                checkCollision(entities[i], entities[j]);
            }
        }
    }
    
    /**
     * @brief Check if an entity can move to a position
     * @param entity The entity to check
     * @param x Target X coordinate
     * @param y Target Y coordinate
     * @return True if the entity can move to the position, false otherwise
     */
    bool canMoveToPosition(Entity* entity, int x, int y) {
        if (!entity) return false;
        
        auto transform = entity->getComponent<TransformComponent>();
        auto collision = entity->getComponent<CollisionComponent>();
        
        if (!transform || !collision) return false;
        
        // Save original position
        int originalX = transform->getX();
        int originalY = transform->getY();
        
        // Temporarily move to the target position
        transform->setPosition(x, y);
        
        // Check for collisions
        bool canMove = true;
        auto entities = getCompatibleEntities();
        
        for (auto otherEntity : entities) {
            if (otherEntity == entity) continue;
            
            auto otherCollision = otherEntity->getComponent<CollisionComponent>();
            if (!otherCollision || !otherCollision->isSolid()) continue;
            
            if (checkCollision(entity, otherEntity, false)) {
                canMove = false;
                break;
            }
        }
        
        // Restore original position
        transform->setPosition(originalX, originalY);
        
        return canMove;
    }
    
    /**
     * @brief Get the system name
     * @return Name of the system
     */
    std::string getName() const override {
        return "CollisionSystem";
    }

private:
    /**
     * @brief Check for a collision between two entities
     * @param entity1 First entity
     * @param entity2 Second entity
     * @param notifyCollision Whether to notify the entities of the collision
     * @return True if a collision occurred, false otherwise
     */
    bool checkCollision(Entity* entity1, Entity* entity2, bool notifyCollision = true) {
        auto transform1 = entity1->getComponent<TransformComponent>();
        auto collision1 = entity1->getComponent<CollisionComponent>();
        
        auto transform2 = entity2->getComponent<TransformComponent>();
        auto collision2 = entity2->getComponent<CollisionComponent>();
        
        // Create rectangles for collision detection
        SDL_Rect rect1 = {
            transform1->getX(),
            transform1->getY(),
            collision1->getWidth(),
            collision1->getHeight()
        };
        
        SDL_Rect rect2 = {
            transform2->getX(),
            transform2->getY(),
            collision2->getWidth(),
            collision2->getHeight()
        };
        
        // Check for intersection
        if (SDL_HasIntersection(&rect1, &rect2)) {
            // Notify entities of collision if requested
            if (notifyCollision) {
                collision1->handleCollision(entity2);
                collision2->handleCollision(entity1);
            }
            return true;
        }
        
        return false;
    }
};

#endif // DENDY_TANKS_COLLISION_SYSTEM_H