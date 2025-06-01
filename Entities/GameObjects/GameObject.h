


#ifndef SSDL_GAMEOBJECT_H
#define SSDL_GAMEOBJECT_H

#include <memory>
#include "Position.h"
#include "StepResult.h"

/**
 * @brief GameObject structure represents an object in the game world.
 */
struct GameObject {
    /** Type of game object */
    enum class Type{
        UNDEFINED = 0x00000000, // Explicitly set to 0
        SPACE = 0x01000000,     // Explicitly set to 1 (matching test expectations)
        PLAYER,
        ENEMY,
        WALL,
        PROJECTILE
    };

    GameObject();
    GameObject(const Position &pos, Type typo);
    ~GameObject();

    Type type_;
    [[nodiscard]] const Positions &getPositions() const;
    StepResult step(); // Returns StepResult instead of void
    bool isAlive() const;

protected:
    Positions positions_;

private:
};

#endif //SSDL_GAMEOBJECT_H

