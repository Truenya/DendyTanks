#ifndef DENDY_TANKS_ENTITY_MANAGER_H
#define DENDY_TANKS_ENTITY_MANAGER_H

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <typeindex>
#include "Entity.h"

/**
 * @class EntityManager
 * @brief Manages all entities in the game
 * 
 * This class is responsible for creating, updating, and destroying entities.
 * It also provides methods for querying entities based on components.
 */
class EntityManager {
public:
    /**
     * @brief Get the singleton instance of the EntityManager
     * @return Reference to the EntityManager instance
     */
    static EntityManager& getInstance() {
        static EntityManager instance;
        return instance;
    }
    
    /**
     * @brief Create a new entity
     * @param id Unique identifier for the entity
     * @return Pointer to the created entity
     */
    Entity* createEntity(const std::string& id) {
        // Check if entity with this ID already exists
        if (entities_.find(id) != entities_.end()) {
            return entities_[id].get();
        }
        
        // Create the entity
        auto entity = std::make_unique<Entity>(id);
        Entity* entityPtr = entity.get();
        
        // Store the entity
        entities_[id] = std::move(entity);
        
        return entityPtr;
    }
    
    /**
     * @brief Get an entity by ID
     * @param id The entity ID
     * @return Pointer to the entity, or nullptr if not found
     */
    Entity* getEntity(const std::string& id) const {
        auto it = entities_.find(id);
        if (it != entities_.end()) {
            return it->second.get();
        }
        return nullptr;
    }
    
    /**
     * @brief Remove an entity by ID
     * @param id The entity ID
     */
    void removeEntity(const std::string& id) {
        entities_.erase(id);
    }
    
    /**
     * @brief Get all entities
     * @return Vector of pointers to all entities
     */
    std::vector<Entity*> getAllEntities() const {
        std::vector<Entity*> result;
        result.reserve(entities_.size());
        
        for (const auto& [id, entity] : entities_) {
            result.push_back(entity.get());
        }
        
        return result;
    }
    
    /**
     * @brief Get entities with a specific component
     * @tparam T The component type
     * @return Vector of pointers to entities with the component
     */
    template<typename T>
    std::vector<Entity*> getEntitiesWithComponent() const {
        std::vector<Entity*> result;
        
        for (const auto& [id, entity] : entities_) {
            if (entity->hasComponent<T>()) {
                result.push_back(entity.get());
            }
        }
        
        return result;
    }
    
    /**
     * @brief Update all entities
     * @param deltaTime Time elapsed since the last update
     */
    void update(float deltaTime) {
        for (auto& [id, entity] : entities_) {
            if (entity->isActive()) {
                entity->update(deltaTime);
            }
        }
    }
    
    /**
     * @brief Clear all entities
     */
    void clear() {
        entities_.clear();
    }

private:
    // Private constructor for singleton pattern
    EntityManager() = default;
    
    // Prevent copying and assignment
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    
    // Entity storage
    std::unordered_map<std::string, std::unique_ptr<Entity>> entities_;
};

#endif // DENDY_TANKS_ENTITY_MANAGER_H