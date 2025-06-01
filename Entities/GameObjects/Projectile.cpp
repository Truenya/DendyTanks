




#include "Projectile.h"

Projectile::Projectile(const Position& pos) : position_(pos) {}

Projectile::~Projectile() {}

Position Projectile::getPos() const {
    return position_;
}

void Projectile::setPos(const Position& pos) {
    position_ = pos;
}




