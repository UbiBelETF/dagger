#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct SimpleCollision
{
    Vector2 size;
    Vector2 pivot {-0.5f, -0.5f};

    //PhisicsResolver* resolverFunction;

    bool IsCollided(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther)
    {
        Vector2 p(pos.x + pivot.x * size.x, pos.y + pivot.y * size.y);
        Vector2 p2(posOther.x + other.pivot.x * other.size.x, posOther.y + other.pivot.y * other.size.y);

        if (p.x < p2.x + other.size.x &&
            p.x + size.x > p2.x &&
            p.y < p2.y + other.size.y &&
            p.y + size.y > p2.y) 
        {
            return true;
        }

        return false;
    }

    Vector3 GetCollisionCenter(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther)
    {
        Vector3 res;

        Vector2 p(pos.x + pivot.x * size.x, pos.y + pivot.y * size.y);
        Vector2 p2(posOther.x + other.pivot.x * other.size.x, posOther.y + other.pivot.y * other.size.y);

        res.x = (p.x + size.x / 2.f + p2.x + other.size.x) / 2.f;
        res.y = (p.y + size.y / 2.f + p2.y + other.size.y) / 2.f;
        res.z = 1.f;

        return res;
    }
};

class SimpleCollisionsSystem : public System
{
    inline String SystemName() { return "Simple Collisions System"; }

    void Run() override;
};
