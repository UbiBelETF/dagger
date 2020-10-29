#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct SimpleCollision
{
    Vector2 size;
    Vector2 pivot {-0.5f, -0.5f};

    bool colided = false;
    entt::entity colidedWith;

    bool IsCollided(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther);

    // return (0,1) if collision happen by y, (1,0) if collision happen by x
    Vector2 GetCollisionSides(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther);

    Vector3 GetCollisionCenter(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther);
};

class SimpleCollisionsSystem : public System
{
public:
    inline String SystemName() { return "Simple Collisions System"; }

    void Run() override;
};
