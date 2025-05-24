#ifndef DENDY_TANKS_TRANSFORM_COMPONENT_H
#define DENDY_TANKS_TRANSFORM_COMPONENT_H

#include "../Component.h"

/**
 * @class TransformComponent
 * @brief Component that represents the position and direction of an entity
 */
class TransformComponent : public Component {
public:
    // Direction enum
    enum class Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT,
        NONE
    };
    
    /**
     * @brief Constructor
     * @param x X coordinate
     * @param y Y coordinate
     * @param direction Direction the entity is facing
     */
    TransformComponent(int x = 0, int y = 0, Direction direction = Direction::UP)
        : x_(x), y_(y), direction_(direction) {}
    
    /**
     * @brief Get the X coordinate
     * @return X coordinate
     */
    int getX() const { return x_; }
    
    /**
     * @brief Get the Y coordinate
     * @return Y coordinate
     */
    int getY() const { return y_; }
    
    /**
     * @brief Get the direction
     * @return Direction
     */
    Direction getDirection() const { return direction_; }
    
    /**
     * @brief Set the X coordinate
     * @param x New X coordinate
     */
    void setX(int x) { x_ = x; }
    
    /**
     * @brief Set the Y coordinate
     * @param y New Y coordinate
     */
    void setY(int y) { y_ = y; }
    
    /**
     * @brief Set the position
     * @param x New X coordinate
     * @param y New Y coordinate
     */
    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }
    
    /**
     * @brief Set the direction
     * @param direction New direction
     */
    void setDirection(Direction direction) { direction_ = direction; }
    
    /**
     * @brief Move in the current direction
     * @param distance Distance to move
     */
    void moveInDirection(int distance = 1) {
        switch (direction_) {
            case Direction::UP:
                y_ -= distance;
                break;
            case Direction::RIGHT:
                x_ += distance;
                break;
            case Direction::DOWN:
                y_ += distance;
                break;
            case Direction::LEFT:
                x_ -= distance;
                break;
            case Direction::NONE:
                break;
        }
    }
    
    /**
     * @brief Convert a Position::Direction to TransformComponent::Direction
     * @param direction Position::Direction value
     * @return Equivalent TransformComponent::Direction value
     */
    static Direction fromPositionDirection(int direction) {
        switch (direction) {
            case 0: return Direction::UP;
            case 1: return Direction::RIGHT;
            case 2: return Direction::DOWN;
            case 3: return Direction::LEFT;
            default: return Direction::NONE;
        }
    }
    
    /**
     * @brief Convert a TransformComponent::Direction to Position::Direction
     * @param direction TransformComponent::Direction value
     * @return Equivalent Position::Direction value
     */
    static int toPositionDirection(Direction direction) {
        switch (direction) {
            case Direction::UP: return 0;
            case Direction::RIGHT: return 1;
            case Direction::DOWN: return 2;
            case Direction::LEFT: return 3;
            default: return 4; // NONE
        }
    }

private:
    int x_;
    int y_;
    Direction direction_;
};

#endif // DENDY_TANKS_TRANSFORM_COMPONENT_H