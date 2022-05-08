//
// Created by true on 2022-05-01.
//

#ifndef SSDL_PARTICLESSYSTEM_H
#define SSDL_PARTICLESSYSTEM_H
#include <vector>
#include <set>
#include "../../Entities/GameObjects/BaseGameObject.h"
#include <thread>
typedef std::vector<std::pair<Position,Position>> RenderShootInfo;

typedef std::set<int> Keys;
typedef std::vector<BaseGameObject*> ProjectileByDistance;

struct ProjectileSystem{
	ProjectileByDistance projectiles_;
	int count_;
	void init();
	void insert(BaseGameObject* object);
	void clear();
	void step();
private:
	static constexpr size_t PROJECTILE_VALUES_SIZE = 50;
};

typedef std::vector<ProjectileSystem> Projectiles;

struct ParticlesSystem {
    Keys keys_;
    Projectiles projectilesByDistances_;
	RenderShootInfo currentPositions_;
	void init();
	void insertByDistance(int distance,BaseGameObject* object);
	void step();
	[[nodiscard]]RenderShootInfo getCurrentPositions() const ;
private:
	static constexpr size_t PROJECTILE_KEYS_SIZE = 50;
};

#endif //SSDL_PARTICLESSYSTEM_H