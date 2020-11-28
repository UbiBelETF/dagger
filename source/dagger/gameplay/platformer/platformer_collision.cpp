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
        //it2++;
        while (it2 != view.end())
        {
            auto& col = view.get<PlatformerCollision>(*it2);
            auto& ch = view.get<PlatformerCharacter>(*it2);
            auto& tr = view.get<Transform>(*it2);

            if (ch.id != character.id)
            {
                if (collision.collidesWith[col.entityType])
                {
                    // Perform the collision check
                    CollisionInfo collisionInfo = collision.GetCollisionInfo(transform.position, col, tr.position);

                    if (collisionInfo.hasCollided)
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
                    else
                    {
                        LimitPlayerMovement(character, collisionInfo);
                    }
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

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x &&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        collisionInfo.hasCollided = true;
        
        if ((p.x + size.x) > p2.x && ((p2.x+other_.size.x) > (p.x + size.x)))
        {
            if ((abs(p.x + size.x - p2.x) < abs(p.y - p2.y - other_.size.y)) && ((abs(p.x + size.x - p2.x) < abs(p.y + size.y - p2.y))))
            {
                // bumped into other_ from the left side
                collisionInfo.collisionSide = CollisionSide::RIGHT;
            }
        }
        

        if (p.x <= (p2.x + other_.size.x) && p.x >= p2.x)
        {
            if ((abs(p.x - p2.x - other_.size.x) < abs(p.y - p2.y - other_.size.y)) && ((abs(p.x - p2.x - other_.size.x) < abs(p.y + size.y - p2.y))))
            {
                // bumped into other_ from the right side
                collisionInfo.collisionSide = CollisionSide::LEFT;
            }
        }
        

        if ((p.y + size.y) >= p2.y && (p2.y + other_.size.y) >= (p.y + size.y))
        {
            if ((abs(p.y + size.y - p2.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y + size.y - p2.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the bottom side
                collisionInfo.collisionSide = CollisionSide::BOTTOM;
            }
        }

        if (p.y <= (p2.y + other_.size.y) && p.y >= p2.y)
        {
            if ((abs(p.y - p2.y - other_.size.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y - p2.y - other_.size.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the top side
                collisionInfo.collisionSide = CollisionSide::TOP;
            }
        }

  }
  else 
  {
      collisionInfo.hasCollided = false;
      collisionInfo.collisionSide = CollisionSide::NONE;
  }

    return collisionInfo;
}

void PlatformerCollisionSystem::LimitPlayerMovement(PlatformerCharacter& character_, CollisionInfo info_)
{
    character_.canGoRight = (info_.collisionSide == CollisionSide::RIGHT) ? false : true;//character_.canGoRight;
    character_.canGoLeft = (info_.collisionSide == CollisionSide::LEFT) ? false : true; //character_.canGoLeft;
    character_.canGoUp = (info_.collisionSide == CollisionSide::TOP) ? false : true; //character_.canGoUp;
    character_.canGoDown = (info_.collisionSide == CollisionSide::BOTTOM) ? false : true; //character_.canGoDown;
}