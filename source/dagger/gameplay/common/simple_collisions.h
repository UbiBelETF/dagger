#pragma once
#include "core/system.h"
#include "core/core.h"

#include <list>

using namespace dagger;

enum class EHitbox {
    Rectangular = 1,
    Circular = 2,
};

struct SimpleCollision
{
    Vector2 size; // If the hitbox is rectangular, size represents height and width, if circular, diameter and angle of coverage (currently unused)
    Vector2 pivot {-0.5f, -0.5f};

    //Float32 startAngleIfCircular; Possible future upgrade; hitboxes not being full circles

    EHitbox shape = EHitbox::Rectangular;

    bool colided = false;
    entt::entity colidedWith; // Kept for compatibility with the current code
    std::list<entt::entity> colisions;

    bool IsCollided(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_);

    // return (0,1) if collision happen by y, (1,0) if collision happen by x
    Vector2 GetCollisionSides(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_);

    Vector3 GetCollisionCenter(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_);
};

class SimpleCollisionsSystem : public System
{
public:
    inline String SystemName() { return "Simple Collisions System"; }

    void Run() override;
};
