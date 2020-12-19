#pragma once
#include "core/system.h"
#include <list>
#include <iterator>
#include "core/core.h"

#include "core/game/transforms.h"

using namespace dagger;

namespace plight {
    struct PlightCollision
    {
        Vector2 size;
        Vector2 pivot{ -0.5f, -0.5f };

        Transform last_pos= Transform(Vector3( -999,-999,-999 ));
        Vector3 place;

        bool colided = false;
        std::list<entt::entity> colidedWith;

        void Setup();

        bool IsCollided(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        bool IsCollidedSAT(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        bool IsCollidedSweptSAT(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        Vector2* GetSweptObject(int& len_, const PlightCollision& col_, const Vector3& pos_);

        // return (0,1) if collision happen by y, (1,0) if collision happen by x
        Vector2 GetCollisionSides(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);

        Vector3 GetCollisionCenter(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_);
    };

    class PlightCollisionsSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Collisions System"; }
        inline void SpinUp() {
            Engine::Registry().on_construct<PlightCollision>().connect<entt::invoke<&PlightCollision::Setup>>();
        }

        void Run() override;
    };
}