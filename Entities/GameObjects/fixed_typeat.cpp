

#include "GameWorld.h"
#include <cassert>

// This is a simple implementation of typeAt that should satisfy the tests
// The key issue seems to be with how we're handling enumerated values for GameObject::Type

GameObject::Type GameWorld::typeAt (Position pos)
{
    // For invalid positions, return SPACE as per test expectations in TypeAt test case
    if ((pos.x_ < 0) || (pos.y_ < 0)) {
        return GameObject::Type::SPACE;
    }

    // For valid positions within bounds
    ssize_t x = static_cast<ssize_t>(pos.x_);
    ssize_t y = static_cast<ssize_t>(pos.y_);

    if(x >= field_.size() || y >= field_[0].size()) {
        return GameObject::Type::SPACE;
    }

    // Check if position is valid according to Position's own validation methods
    if (!pos.isValid() || !pos.isValidByWorldSize({static_cast<ssize_t>(field_.size()), static_cast<ssize_t>(field_[0].size())})) {
        return GameObject::Type::SPACE;
    }

    // Return the actual type at this position for all other cases
    return field_[x][y];
}

