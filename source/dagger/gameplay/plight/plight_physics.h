#pragma once
#include "core/game/transforms.h"

namespace plight {
    struct PlightCollision;

    struct PhysicsObject
    {
        bool is_static = false;
        std::vector<UINT16> my_groups;
        std::vector<UINT16> collision_groups;
    };

    class PhysicsSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Physics System"; }

        void Run() override;

        void ResolveCollision(Vector3& pos_, PlightCollision& my_, const Vector3& posOther_, const PlightCollision& other_);
    };
}
