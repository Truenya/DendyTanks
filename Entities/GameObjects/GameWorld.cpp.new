
#include "GameWorld.h"
#include <cassert>
#include <stdexcept>

GameWorld::GameWorld(unsigned int x_dim, unsigned int y_dim)
{
	if(x_dim > 10000 || y_dim > 10000)
		throw std::logic_error ("trying to construct too big world");
	field_.resize(x_dim);
	for (auto &row: field_) {
		row.resize(y_dim);
		for (auto &point: row) {
			point = GameObject::Type::SPACE;
		}
	}
	projectiles_.init(100);
}

GameWorld::~GameWorld()
{
}

void GameWorld::addGameObject(const std::shared_ptr<const GameObject> obj)
{
    assert(obj->pos().isValidByWorldSize({static_cast<ssize_t>(field_.size()), static_cast<ssize_t>(field_[0].size())}));
    field_[obj->pos().x_][obj->pos().y_] = obj->type_;
}

StepResult GameWorld::step(std::vector<std::shared_ptr<GameObject>> &objects)
{
    /*std::sort(objects.begin(), objects.end(),
              [](const std::shared_ptr<GameObject> &a, const std::shared_ptr<GameObject> &b) {
                  return a->pos().z_ < b->pos().z_;
              });*/
    for (auto& obj: objects) {
        if (!obj->isAlive())
            continue;
        StepResult r = obj->step();
        switch(r.ret_) {
            case StepReturn::SUCCESS:
                break;
            //case StepReturn::UNDEFINED_BEHAVIOR:
            //    return {StepReturn::UNDEFINED_BEHAVIOR};
            default:
                return {StepReturn::UNDEFINED_BEHAVIOR};
        }
    }
    projectiles_.step();
    return {StepReturn::SUCCESS};
}

void GameWorld::addProjectile(const std::shared_ptr<const Projectile> obj)
{
    projectiles_.add(obj);
}

std::vector<std::shared_ptr<Projectile>> &GameWorld::getProjectiles()
{
    return projectiles_.get();
}

StepResult GameWorld::stepProjectiles (std::vector<std::shared_ptr<GameObject>> &objects)
{
    StepResult result = projectiles_.step();
    if(result.ret_ == StepReturn::UNDEFINED_BEHAVIOR)
        return result;
    for(auto& obj: objects) {
        StepResult r = obj->step();
        switch(r.ret_) {
            case StepReturn::SUCCESS:
                break;
            //case StepReturn::UNDEFINED_BEHAVIOR:
            //    return {StepReturn::UNDEFINED_BEHAVIOR,prev_pos};
            default:
                return {StepReturn::UNDEFINED_BEHAVIOR};
        }
    }
    projectiles_.step();
    return {StepReturn::SUCCESS};
}

bool GameWorld::positionIsFree (Position pos) const
{
    if ((pos.x_ < 0) || (pos.y_ < 0))
        return false;
    if (!pos.isValidByWorldSize({static_cast<ssize_t>(field_.size()), static_cast<ssize_t>(field_[0].size())}))
        return false;

    switch(at(pos)) {
        case GameObject::Type::SPACE:
            return true;
        default:
            return false;
    }
}

GameObject::Type GameWorld::at (Position pos) const
{
    if ((pos.x_ < 0) || (pos.y_ < 0))
        return GameObject::Type::UNDEFINED;

    ssize_t x = static_cast<ssize_t>(pos.x_);
    ssize_t y = static_cast<ssize_t>(pos.y_);

    if(x >= field_.size() ||
       y >= field_[0].size())
        return GameObject::Type::UNDEFINED;
    return field_[x][y];
}

GameObject::Type GameWorld::typeAt (Position pos)
{
	if ((pos.x_ < 0) || (pos.y_ < 0))
		return GameObject::Type::SPACE;

	if (pos.isValid() && pos.isValidByWorldSize ({static_cast<ssize_t>(field_.size()),static_cast<ssize_t>(field_[0].size()),0,Position::Direction::TOP}))
	{
		// Ensure we return SPACE for any valid position within the world
		return GameObject::Type::SPACE;
	}

	return GameObject::Type::SPACE;
}
