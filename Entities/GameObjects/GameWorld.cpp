


#include "GameWorld.h"
#include <cassert>
#include <stdexcept>
#include <uuid/uuid.h>  // Include UUID header for uuid() function

// Forward declaration of Projectile class
class Projectile;

GameWorld::GameWorld(unsigned int x_dim, unsigned int y_dim) {
    if(x_dim > 10000 || y_dim > 10000)
        throw std::logic_error ("trying to construct too big world");

    field_.resize(x_dim);
    for (auto &row: field_) {
        row.resize(y_dim);
        for (auto &point: row) {
            point = GameObject::Type::SPACE;
        }
    }

    projectiles_.init(100);  // Initialize the projectile manager
}

GameWorld::~GameWorld() {}

Position GameWorld::size() {
    return {static_cast<int>(field_.size()), static_cast<int>(field_[0].size())};
}

// Add game object to world at its position
void GameWorld::addGameObject(const std::shared_ptr<const GameObject> obj) {
    assert(obj->getPositions().curPos_.isValidByWorldSize({static_cast<ssize_t>(field_.size()),
                                          static_cast<ssize_t>(field_[0].size())}));
    field_[obj->getPositions().curPos_.x_][obj->getPositions().curPos_.y_] = obj->type_;
}

// Step through all game objects and projectiles
StepResult GameWorld::step(std::vector<std::shared_ptr<GameObject>> &objects) {
    for (auto& obj: objects) {
        if (!obj->isAlive())  // Check if object is alive first
            continue;

        StepResult r = obj->step();  // Get step result from the object
        if(r.return_code != StepResult::ReturnCode::SUCCESS)
            return {StepResult::ReturnCode::UNDEFINED_BEHAVIOR};
    }

    // Process projectiles - just call step on each one (dummy implementation for now)
    auto projList = projectiles_.apply();  // Get all projectiles
    for(auto& p : projList) {
        // Step logic for each projectile would go here
    }

    return {StepResult::ReturnCode::SUCCESS};  // Success if all objects processed correctly
}

// Add a projectile to the world's projectile manager
void GameWorld::addProjectile(std::shared_ptr<const Projectile> projectile) {
    Position pos = projectile->getPos();  // Get position from projectile
    projectiles_.add(pos);  // Add to projectile manager
}

// Retrieve all projectiles currently managed by the world
std::vector<std::shared_ptr<Projectile>> GameWorld::getProjectiles() {
    std::vector<std::shared_ptr<Projectile>> result;

    auto posList = projectiles_.apply();  // Get all positions from projectiles_
    for (const auto& pos : posList) {
        // Create a projectile pointer and add to result vector
        result.push_back(std::make_shared<Projectile>(pos));
    }

    return result;
}

// Step specifically just the projectiles - dummy implementation
StepResult GameWorld::stepProjectiles(std::vector<std::shared_ptr<GameObject>> &objects) {
    // Process all projectiles first (dummy implementation)
    auto projList = projectiles_.apply();
    for(auto& p : projList) {
        // Step logic for each projectile would go here
    }

    // Then step through game objects
    for(auto& obj: objects) {
        StepResult r = obj->step();  // Get step result from object
        if(r.return_code != StepResult::ReturnCode::SUCCESS)
            return {StepResult::ReturnCode::UNDEFINED_BEHAVIOR};
    }

    return {StepResult::ReturnCode::SUCCESS};  // Success if all processed correctly
}

// Check if a position is free (has SPACE type)
// Const qualifier means this method can be called on const GameWorld instances
bool GameWorld::positionIsFree(Position pos) const {
    if ((pos.x_ < 0) || (pos.y_ < 0))
        return false;

    // Make sure the position is within bounds
    if (!pos.isValidByWorldSize({static_cast<ssize_t>(field_.size()),
                                static_cast<ssize_t>(field_[0].size())}))
        return false;

    // Check type at this position and return true only for SPACE
    GameObject::Type type = const_at(pos);  // Use const version of at()
    return type == GameObject::Type::SPACE;
}

// Get reference to the game object type at a specific position (const version)
GameObject::Type GameWorld::typeAt(Position pos) const {
    // Handle negative or invalid positions
    if ((pos.x_ < 0) || (pos.y_ < 0)) {
        return GameObject::Type::SPACE;
    }

    ssize_t x = static_cast<ssize_t>(pos.x_);
    ssize_t y = static_cast<ssize_t>(pos.y_);

    // Check array bounds
    if(x >= field_.size() || y >= field_[0].size()) {
        return GameObject::Type::SPACE;
    }

    // Check position validity
    if (!pos.isValidByWorldSize({static_cast<ssize_t>(field_.size()),
                                 static_cast<ssize_t>(field_[0].size())})) {
        return GameObject::Type::SPACE;
    }

    // Return the actual type for valid positions within bounds
    return field_[x][y];
}

// Get reference to the game object type at a specific position (non-const version)
GameObject::Type &GameWorld::at(Position pos) {
    ssize_t x = static_cast<ssize_t>(pos.x_);
    ssize_t y = static_cast<ssize_t>(pos.y_);

    // Check array bounds before access
    if(x < 0 || y < 0 || x >= field_.size() || y >= field_[0].size()) {
        throw std::out_of_range("Position out of range");
    }

    return field_[x][y];
}

// Const version of at() method for use with const GameWorld objects
GameObject::Type GameWorld::const_at(Position pos) const {
    ssize_t x = static_cast<ssize_t>(pos.x_);
    ssize_t y = static_cast<ssize_t>(pos.y_);

    // Check array bounds before access
    if(x < 0 || y < 0 || x >= field_.size() || y >= field_[0].size()) {
        throw std::out_of_range("Position out of range");
    }

    return field_[x][y];
}

// Add a tank to the world and return its ID
std::string GameWorld::addTank(const GameObject& tank) {
    // Generate unique ID for tank (using UUID library)
    uuid_t id;
    uuid_generate(id);

    char uuid_str[37];  // UUID strings are 36 characters plus null terminator
    uuid_unparse(id, uuid_str);

    tanks_.emplace(std::string(uuid_str), tank);

    // Set as current player UUID
    my_uuid() = std::string(uuid_str);

    // Place tank in the world at its position
    Position pos = tank.getPositions().curPos_;
    field_[pos.x_][pos.y_] = tank.type_;

    return std::string(uuid_str);  // Return the tank's ID
}


