



#ifndef SSDL_PROJECTILE_H
#define SSDL_PROJECTILE_H

#include "Position.h"
#include "GameObject.h"

class Projectile {
public:
    explicit Projectile(const Position& pos = {-1, -1});
    ~Projectile();

    Position getPos() const;
    void setPos(const Position& pos);

private:
    Position position_;
};

#endif // SSDL_PROJECTILE_H


