#pragma once
#include "core/game/transforms.h"

namespace plight {
    struct PlightCollision;

    struct PhysicsObject
    {
        bool is_static = true;
        std::vector<UINT16> my_groups;
        std::vector<UINT16> collision_groups;
    };

    class PhysicsSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Physics System"; }

        void Run() override;

        void ResolveCollision(Vector3& pos_, PlightCollision& myCol_, entt::entity my_, const Vector3& posOther_, PlightCollision& otherCol_, std::list<entt::entity>::iterator& other_);
        void ResolveDynamicCollision(Vector3& pos_, PlightCollision& myCol_, entt::entity my_, Vector3& posOther_, PlightCollision& otherCol_, std::list<entt::entity>::iterator& other_);
    };
}
