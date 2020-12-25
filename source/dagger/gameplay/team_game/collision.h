#pragma once
#include "core/system.h"
#include "core/core.h"

class Transform;

using namespace dagger;

struct Collision
{
    Vector2 size;
    Vector2 pivot{ -0.5f, 0.0f };

    bool colided = false;
    entt::entity colidedWith;

    bool IsCollided(const Vector3& pos_, const Collision& other_, const Vector3& posOther_);

    // return (0,1) if collision happen by y, (1,0) if collision happen by x
    Vector2 GetCollisionSides(const Vector3& pos_, const Collision& other_, const Vector3& posOther_);

    Vector3 GetCollisionCenter(const Vector3& pos_, const Collision& other_, const Vector3& posOther_);
};

namespace CollisionType {
    struct Wall {};
    struct Character {};
    struct Slime {};
}

class CollisionSystem : public System
{
    void ResolveCharWall(Collision& colChar_, Collision& colWall_, Transform& trChar_, Transform& trWall_);
    void ResolveCharChar(Collision& colChar_, Collision& colChar2_, Transform& trChar_, Transform& trChar2_);
public:
    inline String SystemName() { return "Collision System"; }

    void Run() override;
};
