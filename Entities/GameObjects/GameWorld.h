



#ifndef SSDL_GAMEWORLD_H
#define SSDL_GAMEWORLD_H

#include "Entities/Utility.h"
#include "GameObject.h"
#include "memory"
#include "vector"
#include <unordered_map>
#include "StepResult.h"

// Forward declaration of Projectile class to avoid circular dependencies
class Projectile;

struct StepReturn {
    enum ReturnCode {
        SUCCESS,
        MEET_WALL,
        MEET_PLAYER,
        MEET_ENEMY,
        MEET_PROJECTILE,
        OUT_OF_FIELD,
        UNDEFINED_BEHAVIOR
    } ret_{UNDEFINED_BEHAVIOR};
};

class WorldGenerator;
typedef std::vector<std::vector<GameObject::Type>> MyGameWorldField;

struct GameWorld {
    GameWorld(unsigned int x_dim, unsigned int y_dim);
    ~GameWorld();
    Position size();

    std::unordered_map<std::string, GameObject> tanks_;
    StepReturn step(Positions pos);

    // Added methods for test compatibility
    bool positionIsFree(Position pos) const;
    [[nodiscard]] GameObject::Type typeAt(Position pos) const;  // Const version of typeAt
    std::vector<std::shared_ptr<GameObject>> getObjects() const;

    std::string addTank(const GameObject&);
    void addGameObject(const std::shared_ptr<const GameObject> obj);
    void addProjectile(std::shared_ptr<const Projectile> projectile);
    std::vector<std::shared_ptr<Projectile>> getProjectiles();

    StepResult step(std::vector<std::shared_ptr<GameObject>> &objects);
    StepResult stepProjectiles (std::vector<std::shared_ptr<GameObject>> &objects);

private:
    [[nodiscard]] GameObject::Type &at(Position pos);  // Non-const version of at()
    [[nodiscard]] GameObject::Type const_at(Position pos) const;  // Const version of at()

    StepReturn tankStep(const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos);
    StepReturn projectileStep(const Position &prev_pos, GameObject::Type dst_type, Position &dst_pos);

    MyGameWorldField field_;
    ManagedVector<Position> projectiles_;  // Projectile manager
    friend WorldGenerator;

    static std::string& my_uuid() {
        static std::string my_uuid;
        return my_uuid;
    }

    bool playerAlive = true;
};

#endif // SSDL_GAMEWORLD_H


