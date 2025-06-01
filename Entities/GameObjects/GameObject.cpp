

#include "GameObject.h"

// Default constructor
GameObject::GameObject() :
        type_(GameObject::Type::UNDEFINED),
        positions_({{-1, -1, 0, Position::Direction::BOT},
                    {-1, -1, 0, Position::Direction::BOT}}) {}

GameObject::GameObject(const Position& pos, GameObject::Type typo) :
        type_(typo),
        positions_({pos, pos}) {}

GameObject::~GameObject() {}

const Positions &GameObject::getPositions() const {
    return positions_;
}

StepResult GameObject::step() {
    // Implement basic step functionality
    positions_.prevPos_ = positions_.curPos_;
    positions_.curPos_.stepInDirection();
    return {StepResult::ReturnCode::SUCCESS};
}

bool GameObject::isAlive() const {
    // Simple implementation - returns true if position is valid
    return positions_.curPos_.x_ >= 0 && positions_.curPos_.y_ >= 0;
}

