#pragma once
#include "core/system.h"
#include <list>
#include <iterator>
#include "core/core.h"

using namespace dagger;

namespace plight {
    struct PlightCollision
    {
        Vector2 size;
        Vector2 pivot{ -0.5f, -0.5f };

        bool colided = false;
        std::list<entt::entity> colidedWith;

        bool IsCollided(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        // return (0,1) if collision happen by y, (1,0) if collision happen by x
        Vector2 GetCollisionSides(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        Vector3 GetCollisionCenter(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);
    };

    class PlightCollisionsSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Collisions System"; }

        void Run() override;
    };
}