#include "platformer_collision.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/platformer/platformer_controller.h"

using namespace dagger;
using namespace platformer;

void PlatformerCollisionSystem::Run()
{
    auto view = Engine::Registry().view<PlatformerCollision, PlatformerCharacter, Transform>();
    auto it = view.begin();

    while (it != view.end())
    {
        auto& collision = view.get<PlatformerCollision>(*it);
        auto& character = view.get<PlatformerCharacter>(*it);
        auto& transform = view.get<Transform>(*it);

        auto it2 = view.begin();
        while (it2 != view.end())
        {
            auto& col = view.get<PlatformerCollision>(*it2);
            auto& ch = view.get<PlatformerCharacter>(*it2);
            auto& tr = view.get<Transform>(*it2);

            if (ch.id != character.id)
            {
                // Perform the collision check
                CollisionInfo collisionInfo = collision.GetCollisionInfo(transform.position, col, tr.position);

                if (collisionInfo.hasCollided)
                {
                    if (collision.collidesWith[col.entityType])
                    {
                        if (collision.entityType == PlatformerCollisionSystem::playerID)
                        {
                            if (col.entityType == PlatformerCollisionSystem::terrainID ||
                                col.entityType == PlatformerCollisionSystem::playerID)
                            {
                                LimitPlayerMovement(character, collisionInfo);
                            }
                        }

                    }
                }
                else
                {
                    LimitPlayerMovement(character, collisionInfo);
                }
            }
            it2++;
        }
        it++;
    }

}

CollisionInfo PlatformerCollision::GetCollisionInfo(const Vector3& pos_, const PlatformerCollision& other_, const Vector3& posOther_)
{
    CollisionInfo collisionInfo;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (((p.x + size.x) > p2.x && (p2.y + other_.size.y) <= p.y)
        || p.x <= (p2.x + other_.size.x) || p.y > (p2.y + other_.size.y)
        || (p.y + size.y) <= p2.y)
    {
        collisionInfo.hasCollided = true;

        if ((p.x + size.x) > p2.x && ((p2.x+other_.size.x)> (p.x + size.x)))
        {
            // bumped into other_ from the left side
            collisionInfo.collisionSide[0] = true;
        }
        else
        {
            collisionInfo.collisionSide[0] = false;
        }
        if (p.x <= (p2.x + other_.size.x) && p.x>=p2.x)
        {
            // bumped into other_ form the right side 
            collisionInfo.collisionSide[1] = true;
        }
        else
        {
            collisionInfo.collisionSide[1] = false;
        }
        if (p.y > (p2.y + other_.size.y))
        {
            // bumped into other_ from the bottom (ex: jumping)
            collisionInfo.collisionSide[2] = true;
        }
        else
        {
            collisionInfo.collisionSide[2] = false;
        }
        if ((p.y + size.y) <= p2.y)
        {
            // standing on the other_
            collisionInfo.collisionSide[3] = true;
        }
        else
        {
            collisionInfo.collisionSide[3] = false;
        }
    }
    else 
    {
        collisionInfo.hasCollided = false;
        for (auto& col : collisionInfo.collisionSide)
        {
            col = false;
        }
    }

    return collisionInfo;
}

void PlatformerCollisionSystem::LimitPlayerMovement(PlatformerCharacter& character_, CollisionInfo info_)
{
    character_.canGoRight = !info_.collisionSide[0];
    character_.canGoLeft = !info_.collisionSide[1];
    character_.canGoUp = !info_.collisionSide[2];
    character_.canGoDown = !info_.collisionSide[3];
}