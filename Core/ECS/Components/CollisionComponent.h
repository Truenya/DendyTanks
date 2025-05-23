#ifndef DENDY_TANKS_COLLISION_COMPONENT_H
#define DENDY_TANKS_COLLISION_COMPONENT_H

#include "../Component.h"
#include <string>
#include <functional>

/**
 * @class CollisionComponent
 * @brief Component that represents the collision properties of an entity
 */
class CollisionComponent : public Component {
public:
    /**
     * @brief Constructor
     * @param width Width of the collision box
     * @param height Height of the collision box
     * @param solid Whether the entity is solid (blocks movement)
     * @param tag Collision tag for filtering
     */
    CollisionComponent(int width = 32, int height = 32, bool solid = true, const std::string& tag = "default")
        : width_(width), height_(height), solid_(solid), tag_(tag) {}
    
    /**
     * @brief Get the width
     * @return Width of the collision box
     */
    int getWidth() const { return width_; }
    
    /**
     * @brief Get the height
     * @return Height of the collision box
     */
    int getHeight() const { return height_; }
    
    /**
     * @brief Set the size
     * @param width New width
     * @param height New height
     */
    void setSize(int width, int height) {
        width_ = width;
        height_ = height;
    }
    
    /**
     * @brief Check if the entity is solid
     * @return True if solid, false otherwise
     */
    bool isSolid() const { return solid_; }
    
    /**
     * @brief Set whether the entity is solid
     * @param solid True to make solid, false otherwise
     */
    void setSolid(bool solid) { solid_ = solid; }
    
    /**
     * @brief Get the collision tag
     * @return Collision tag
     */
    const std::string& getTag() const { return tag_; }
    
    /**
     * @brief Set the collision tag
     * @param tag New collision tag
     */
    void setTag(const std::string& tag) { tag_ = tag; }
    
    /**
     * @brief Set the collision callback
     * @param callback Function to call when a collision occurs
     */
    void setCollisionCallback(std::function<void(Entity*, Entity*)> callback) {
        onCollision_ = callback;
    }
    
    /**
     * @brief Handle a collision
     * @param other The other entity involved in the collision
     */
    void handleCollision(Entity* other) {
        if (onCollision_) {
            onCollision_(getOwner(), other);
        }
    }

private:
    int width_;
    int height_;
    bool solid_;
    std::string tag_;
    std::function<void(Entity*, Entity*)> onCollision_;
};

#endif // DENDY_TANKS_COLLISION_COMPONENT_H