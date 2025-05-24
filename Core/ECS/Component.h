#ifndef DENDY_TANKS_COMPONENT_H
#define DENDY_TANKS_COMPONENT_H

#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

// Forward declaration
class Entity;

/**
 * @class Component
 * @brief Base class for all components in the entity-component system
 * 
 * Components contain data and functionality that can be attached to entities.
 * Each component represents a specific aspect of an entity, such as position,
 * rendering, collision, etc.
 */
class Component {
public:
    /**
     * @brief Constructor
     */
    Component() = default;
    
    /**
     * @brief Virtual destructor
     */
    virtual ~Component() = default;
    
    /**
     * @brief Initialize the component
     * @param owner The entity that owns this component
     */
    virtual void initialize(Entity* owner) {
        owner_ = owner;
    }
    
    /**
     * @brief Update the component
     * @param deltaTime Time elapsed since the last update
     */
    virtual void update(float deltaTime) {}
    
    /**
     * @brief Get the component type
     * @return Type index of the component
     */
    virtual std::type_index getType() const {
        return std::type_index(typeid(*this));
    }
    
    /**
     * @brief Get the component type name
     * @return Name of the component type
     */
    virtual std::string getTypeName() const {
        return typeid(*this).name();
    }
    
    /**
     * @brief Get the entity that owns this component
     * @return Pointer to the owner entity
     */
    Entity* getOwner() const {
        return owner_;
    }

protected:
    // The entity that owns this component
    Entity* owner_ = nullptr;
};

#endif // DENDY_TANKS_COMPONENT_H