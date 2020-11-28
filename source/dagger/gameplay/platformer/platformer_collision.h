#pragma once
#include "core/system.h"
#include "core/core.h"
#include <array>

#include "gameplay/platformer/platformer_controller.h"

using namespace dagger;
using namespace platformer;

enum class CollisionSide {NONE, RIGHT, LEFT, TOP, BOTTOM};

struct CollisionInfo
{
    Bool hasCollided{ false };

    CollisionSide collisionSide = CollisionSide::NONE;
};

struct PlatformerCollision
{
    UInt8 entityType = 0;
    Vector2 size;
    Vector2 pivot{ -0.5f, -0.5f };
    
    // player, terrain, enemies, traps, collectables, weapons
    // For player it would be: false, true, true, true, ???, false
    StaticArray<Bool, 6> collidesWith { true, true, true, true, true, false };

    CollisionInfo GetCollisionInfo(const Vector3& pos_, const PlatformerCollision& other_, const Vector3& posOther_);

};

class PlatformerCollisionSystem : public System
{
public:
    UInt8 playerID{ 0 };
    UInt8 terrainID{ 1 };
    UInt8 enemyID{ 2 };

    inline String SystemName() { return "Platformer Collisions System"; }

    void Run() override;

    void LimitPlayerMovement(PlatformerCharacter& character_, CollisionInfo info_);
};
