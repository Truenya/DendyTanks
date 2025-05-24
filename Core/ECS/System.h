#ifndef DENDY_TANKS_SYSTEM_H
#define DENDY_TANKS_SYSTEM_H

#include <vector>
#include <typeindex>
#include <string>
#include "Entity.h"
#include "EntityManager.h"

/**
 * @class System
 * @brief Base class for all systems in the entity-component system
 * 
 * Systems operate on entities that have specific components. Each system
 * is responsible for a specific aspect of the game, such as rendering,
 * physics, input, etc.
 */
class System {
public:
    /**
     * @brief Constructor
     */
    System() = default;
    
    /**
     * @brief Virtual destructor
     */
    virtual ~System() = default;
    
    /**
     * @brief Initialize the system
     */
    virtual void initialize() {}
    
    /**
     * @brief Update the system
     * @param deltaTime Time elapsed since the last update
     */
    virtual void update(float deltaTime) = 0;
    
    /**
     * @brief Get the system name
     * @return Name of the system
     */
    virtual std::string getName() const {
        return typeid(*this).name();
    }
    
    /**
     * @brief Add a required component type
     * @tparam T The component type
     */
    template<typename T>
    void addRequiredComponent() {
        requiredComponents_.push_back(std::type_index(typeid(T)));
    }
    
    /**
     * @brief Check if an entity has all required components
     * @param entity The entity to check
     * @return True if the entity has all required components, false otherwise
     */
    bool hasRequiredComponents(Entity* entity) const {
        if (!entity) return false;
        
        for (const auto& componentType : requiredComponents_) {
            bool found = false;
            for (const auto& entityComponentType : entity->getComponentTypes()) {
                if (componentType == entityComponentType) {
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                return false;
            }
        }
        
        return true;
    }
    
    /**
     * @brief Get entities that have all required components
     * @return Vector of pointers to entities with all required components
     */
    std::vector<Entity*> getCompatibleEntities() const {
        std::vector<Entity*> result;
        auto allEntities = EntityManager::getInstance().getAllEntities();
        
        for (auto entity : allEntities) {
            if (hasRequiredComponents(entity) && entity->isActive()) {
                result.push_back(entity);
            }
        }
        
        return result;
    }

protected:
    // Required component types
    std::vector<std::type_index> requiredComponents_;
};

#endif // DENDY_TANKS_SYSTEM_H