#ifndef DENDY_TANKS_ENTITY_H
#define DENDY_TANKS_ENTITY_H

#include <memory>
#include <unordered_map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include "Component.h"

/**
 * @class Entity
 * @brief Base class for all game entities in the entity-component system
 * 
 * Entities are containers for components. They represent game objects such as
 * players, enemies, projectiles, etc.
 */
class Entity {
public:
    /**
     * @brief Constructor
     * @param id Unique identifier for the entity
     */
    explicit Entity(const std::string& id) : id_(id), active_(true) {}
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Entity() = default;
    
    /**
     * @brief Initialize the entity
     */
    virtual void initialize() {
        for (auto& [type, component] : components_) {
            component->initialize(this);
        }
    }
    
    /**
     * @brief Update the entity
     * @param deltaTime Time elapsed since the last update
     */
    virtual void update(float deltaTime) {
        if (!active_) return;
        
        for (auto& [type, component] : components_) {
            component->update(deltaTime);
        }
    }
    
    /**
     * @brief Add a component to the entity
     * @tparam T The component type
     * @tparam Args Constructor argument types
     * @param args Constructor arguments
     * @return Pointer to the added component
     */
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        // Create the component
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        T* componentPtr = component.get();
        
        // Store the component
        components_[std::type_index(typeid(T))] = std::move(component);
        componentTypes_.push_back(std::type_index(typeid(T)));
        
        // Initialize the component
        componentPtr->initialize(this);
        
        return componentPtr;
    }
    
    /**
     * @brief Get a component by type
     * @tparam T The component type
     * @return Pointer to the component, or nullptr if not found
     */
    template<typename T>
    T* getComponent() const {
        auto it = components_.find(std::type_index(typeid(T)));
        if (it != components_.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    /**
     * @brief Check if the entity has a component
     * @tparam T The component type
     * @return True if the entity has the component, false otherwise
     */
    template<typename T>
    bool hasComponent() const {
        return components_.find(std::type_index(typeid(T))) != components_.end();
    }
    
    /**
     * @brief Remove a component by type
     * @tparam T The component type
     */
    template<typename T>
    void removeComponent() {
        auto typeIndex = std::type_index(typeid(T));
        components_.erase(typeIndex);
        
        // Remove from component types vector
        auto it = std::find(componentTypes_.begin(), componentTypes_.end(), typeIndex);
        if (it != componentTypes_.end()) {
            componentTypes_.erase(it);
        }
    }
    
    /**
     * @brief Get the entity ID
     * @return The entity ID
     */
    const std::string& getId() const {
        return id_;
    }
    
    /**
     * @brief Set the entity's active state
     * @param active True to activate, false to deactivate
     */
    void setActive(bool active) {
        active_ = active;
    }
    
    /**
     * @brief Check if the entity is active
     * @return True if active, false otherwise
     */
    bool isActive() const {
        return active_;
    }
    
    /**
     * @brief Get all component types
     * @return Vector of component type indices
     */
    const std::vector<std::type_index>& getComponentTypes() const {
        return componentTypes_;
    }

private:
    // Entity ID
    std::string id_;
    
    // Active state
    bool active_;
    
    // Component storage
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
    
    // Component types for iteration
    std::vector<std::type_index> componentTypes_;
};

#endif // DENDY_TANKS_ENTITY_H