#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct CharacterCollision
{
    Vector2 size;
    Vector2 pivot{ -0.5f, 0.0f };

    bool colided = false;
    entt::entity colidedWith;

    bool IsCollided(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_);

    // return (0,1) if collision happen by y, (1,0) if collision happen by x
    Vector2 GetCollisionSides(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_);

    Vector3 GetCollisionCenter(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_);
};

class CharacterCollisionSystem : public System
{
public:
    inline String SystemName() { return "Character Collision System"; }

    static Entity MainCharacterEntity;

    void Run() override;
};
