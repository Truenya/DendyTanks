//
// Created by true on 2022-05-01.
//

#ifndef PARTICLES_SYSTEM_H
#define PARTICLES_SYSTEM_H
#include<vector>
#include "ShootGameObject.h"

typedef std::vector<int> Keys;
typedef std::vector<std::vector<ShootGameObject>> Projectiles;

struct ParticlesSystem {
    Keys keys_;
    Projectiles projectiles_;
}


#endif //PARTICLES_SYSTEM_H