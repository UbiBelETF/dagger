#pragma once
#include "core/system.h"
#include "core/core.h"
#include <array>

using namespace dagger;

namespace team_game
{
    enum class CollisionSide { NONE, RIGHT, LEFT, TOP, BOTTOM };

    enum class CollisionID { PLAYER, TERRAIN, ENEMY, COLLECTABLE, TRAP };

    enum class MovementState { IMMOBILE, UNSTOPPABLE, MOVEABLE };

    struct CollisionInfo
    {
        Bool hasCollided{ false };

        CollisionSide collisionSide = CollisionSide::NONE, collisionSideOther = CollisionSide::NONE;
    };

    struct Collider
    {
        CollisionID entityType = CollisionID::PLAYER;//this is a placeholder and should be changed to not be automatically player
        MovementState state = MovementState::MOVEABLE;

        Vector2 size;
        Vector2 pivot{ -0.5f, -0.5f };

        Bool hasGravity{ false };

        Bool canGoRight{ true };
        Bool canGoLeft{ true };
        Bool canGoUp{ true };
        Bool canGoDown{ true };

        std::vector<entt::entity> listOfEntities;//holds list of entities that this object collided with during this frame
        std::vector<CollisionSide> listOfCollisionSides;//holds the corresponding list of sides of collisions

        // player, terrain, enemies, traps, collectables, weapons
        // For player it would be: false, true, true, true, ???, false
        StaticArray<Bool, 6> collidesWith{ true, true, true, true, true, false };

        CollisionInfo GetCollisionInfo(Vector3& pos_, const Collider& other_, Vector3& posOther_);

    };

    class CollisionSystem : public System
    {
    public:

        inline String SystemName() { return "Platformer Collisions System"; }

        void Run() override;

        void LimitPlayerMovement(Collider& collision_);
    };
}