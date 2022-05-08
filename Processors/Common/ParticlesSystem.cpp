//
// Created by true on 2022-05-06.
//

#include "ParticlesSystem.h"

void ProjectileSystem::step ()
{
	for (int i = 0; i < count_; ++i)
	{
		projectiles_[i]->step();
	}

}

void ProjectileSystem::init ()
{
	projectiles_.resize(PROJECTILE_VALUES_SIZE);
	count_ = 0;
}

void ProjectileSystem::insert (BaseGameObject *object)
{
	projectiles_[count_] = object;
	count_++;
}

void ProjectileSystem::clear ()
{
	count_ = 0;
	std::fill(projectiles_.begin(), projectiles_.end(), nullptr);
}

void ParticlesSystem::step ()
{
	if (keys_.empty())
	{
		return;
	}
	currentPositions_.clear();
	for (auto key: keys_)
	{
		if (key > 0)
		{
			auto next_key = key - 1;
			projectilesByDistances_[key].step();
			for (int i = 0; (i >= 0) && (i < projectilesByDistances_[key].count_); i++)
			{
				auto positions = std::pair<Position, Position>
						{projectilesByDistances_[key].projectiles_[i]->getPositions().prevPos_,
								projectilesByDistances_[key].projectiles_[i]->getPositions().curPos_};
				currentPositions_.emplace_back(positions);
			}
			std::swap(projectilesByDistances_[key], projectilesByDistances_[next_key]);
		}
		else
		{
			projectilesByDistances_[key].clear();
			keys_.erase(key);
		}
	}
	for (const auto &key: keys_)
	{
		if (key > 0)
		{
			keys_.erase(key);
			keys_.insert(key - 1);
		}
	}
}

RenderShootInfo ParticlesSystem::getCurrentPositions () const
{
	return currentPositions_;
}

void ParticlesSystem::insertByDistance (int distance, BaseGameObject *object)
{
	keys_.insert(distance);
	object->type_ = BaseGameObject::Type::PROJECTILE;
	projectilesByDistances_[distance].insert(object);
}

void ParticlesSystem::init ()
{
	projectilesByDistances_.resize(PROJECTILE_KEYS_SIZE);
}