#include "team_game_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "character_controller.h"

using namespace dagger;
using namespace team_game;

SInt32 team_game::Neighborhood(Float32 x_)
{
    auto bucket = (SInt32)(x_ / 250.0f);
    return bucket;
}

void CollisionSystem::SpinUp()
{}

void CollisionSystem::Run()
{
    auto* board = Engine::GetDefaultResource<Map<SInt32, Sequence<Entity>>>();
    if (board->size() == 0 && cachedStatics.size() > 0)
        cachedStatics.clear();

    const auto dynamics = Engine::Registry().view<Collider, Transform, PlayerCharacter>();

    auto iterator = dynamics.begin();

    while (iterator != dynamics.end())
    {
        auto& collision = dynamics.get<Collider>(*iterator);

        collision.listOfEntities.clear();
        collision.listOfCollisionSides.clear();
        collision.canGoLeft = true;
        collision.canGoRight = true;
        collision.canGoUp = true;
        collision.canGoDown = true;
        iterator++;
    }

    auto it = dynamics.begin();

    while (it != dynamics.end())
    {
        Sequence<Entity> okStatics, statics;

        auto& [dynamicCollider, dynamicTransform] = dynamics.get<Collider, Transform>(*it);
        const auto dynamicIndex = Neighborhood(dynamicTransform.position.x);

        if (cachedStatics.find(dynamicIndex) != cachedStatics.end())
        {
            for (SInt32 i = -1; i < 2; i++) // -1, 0, 1
            {
                auto index = dynamicIndex + i;
                if (board->contains(index))
                {
                    for (auto stat : (*board)[index]) statics.push_back(stat);
                }
            }

            cachedStatics[dynamicIndex] = statics;
        }
        else
        {
            statics = cachedStatics[dynamicIndex];
        }

        for (auto id : statics)
        {
            if (Engine::Registry().valid(id) && Engine::Registry().has<Collider, Transform>(id))
                okStatics.push_back(id);
        }

        if (okStatics.size() != statics.size())
        {
            statics = okStatics;
            cachedStatics[dynamicIndex] = statics;
        }

        for (auto id : statics)
        { 
            auto& [staticCollider, staticTransform] = Engine::Registry().get<Collider, Transform>(id);

            if (dynamicCollider.collidesWith[(int)staticCollider.entityType])
            {
                // Perform the collision check
                const CollisionInfo& collisionInfo = dynamicCollider.GetCollisionInfo(dynamicTransform.position, staticCollider, staticTransform.position);

                if (collisionInfo.hasCollided)
                {
                    dynamicCollider.listOfEntities.push_back(id);
                    dynamicCollider.listOfCollisionSides.push_back(collisionInfo.collisionSide);

                    staticCollider.listOfEntities.push_back(*it);
                    staticCollider.listOfCollisionSides.push_back(collisionInfo.collisionSideOther);
                }
            }
        }

        LimitPlayerMovement(dynamicCollider);
        it++;
    }

}

CollisionInfo Collider::GetCollisionInfo(Vector3& pos_, const Collider& other_, Vector3& posOther_)
{
    CollisionInfo collisionInfo;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x &&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        collisionInfo.hasCollided = true;

        if ((p.x + size.x) > p2.x && ((p2.x + other_.size.x) > (p.x + size.x)))
        {
            if ((abs(p.x + size.x - p2.x) < abs(p.y - p2.y - other_.size.y)) && ((abs(p.x + size.x - p2.x) < abs(p.y + size.y - p2.y))))
            {
                // bumped into other_ from the left side
                collisionInfo.collisionSide = CollisionSide::RIGHT;
                if (state == MovementState::MOVEABLE) pos_.x -= p.x + size.x - p2.x - 0.01f;

                collisionInfo.collisionSideOther = CollisionSide::LEFT;
                if (other_.state == MovementState::MOVEABLE) posOther_.x -= p2.x - p.x - size.x + 0.01f;
            }
        }


        if (p.x <= (p2.x + other_.size.x) && p.x >= p2.x)
        {
            if ((abs(p.x - p2.x - other_.size.x) < abs(p.y - p2.y - other_.size.y)) && ((abs(p.x - p2.x - other_.size.x) < abs(p.y + size.y - p2.y))))
            {
                // bumped into other_ from the right side
                collisionInfo.collisionSide = CollisionSide::LEFT;
                if (state == MovementState::MOVEABLE) pos_.x -= p.x - p2.x - other_.size.x + 0.01f;

                collisionInfo.collisionSideOther = CollisionSide::RIGHT;
                if (other_.state == MovementState::MOVEABLE) posOther_.x -= p2.x + other_.size.x - p.x - 0.01f;
            }
        }


        if ((p.y + size.y) >= p2.y && (p2.y + other_.size.y) >= (p.y + size.y))
        {
            if ((abs(p.y + size.y - p2.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y + size.y - p2.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the bottom side
                collisionInfo.collisionSide = CollisionSide::TOP;
                if (state == MovementState::MOVEABLE) pos_.y -= p.y + size.y - p2.y - 0.01f;

                collisionInfo.collisionSideOther = CollisionSide::BOTTOM;
                if (other_.state == MovementState::MOVEABLE) posOther_.y -= p2.y - p.y - size.y + 0.01f;
            }
        }

        if (p.y <= (p2.y + other_.size.y) && p.y >= p2.y)
        {
            if ((abs(p.y - p2.y - other_.size.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y - p2.y - other_.size.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the top side
                collisionInfo.collisionSide = CollisionSide::BOTTOM;
                if (state == MovementState::MOVEABLE) pos_.y -= p.y - p2.y - other_.size.y + 0.01f;

                collisionInfo.collisionSideOther = CollisionSide::TOP;
                if (other_.state == MovementState::MOVEABLE) posOther_.y -= p2.y + other_.size.y - p.y - 0.01f;
            }
        }

    }
    else
    {
        collisionInfo.hasCollided = false;
        collisionInfo.collisionSide = CollisionSide::NONE;
        collisionInfo.collisionSideOther = CollisionSide::NONE;
    }

    return collisionInfo;
}

void CollisionSystem::LimitPlayerMovement(Collider& collision_)
{
    if (!collision_.listOfEntities.empty())
    {
        for (int i = 0; i < collision_.listOfEntities.size(); i++)
        {
            //character_.canGoRight;
            collision_.canGoRight = (collision_.listOfCollisionSides[i] == CollisionSide::RIGHT) ? false : collision_.canGoRight;
            //character_.canGoLeft;
            collision_.canGoLeft = (collision_.listOfCollisionSides[i] == CollisionSide::LEFT) ? false : collision_.canGoLeft; 
            //character_.canGoUp;
            collision_.canGoUp = (collision_.listOfCollisionSides[i] == CollisionSide::TOP) ? false : collision_.canGoUp; 
            //character_.canGoDown;
            collision_.canGoDown = (collision_.listOfCollisionSides[i] == CollisionSide::BOTTOM) ? false : collision_.canGoDown;
        }
    }
}