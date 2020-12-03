#include "team_game_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/team_game_controller.h"

using namespace dagger;
using namespace team_game;

void PlatformerCollisionSystem::Run()
{
    auto view = Engine::Registry().view<PlatformerCollision, PlatformerCharacter, Transform>();
    auto iterator = view.begin();

    while (iterator != view.end())//this for loop clears all previous collision data
    {
        auto& collision = view.get<PlatformerCollision>(*iterator);
        auto& character = view.get<PlatformerCharacter>(*iterator);

        collision.listOfEntities.clear();
        collision.listOfCollisionSides.clear();
        character.canGoLeft = true;
        character.canGoRight = true;
        character.canGoUp = true;
        character.canGoDown = true;
        iterator++;
    }

    auto it = view.begin();

    while (it != view.end())
    {
        auto& collision = view.get<PlatformerCollision>(*it);
        auto& character = view.get<PlatformerCharacter>(*it);
        auto& transform = view.get<Transform>(*it);

        Vector2 p{};

        auto it2 = it;
        it2++;
        while (it2 != view.end())
        {
            auto& col = view.get<PlatformerCollision>(*it2);
            auto& ch = view.get<PlatformerCharacter>(*it2);
            auto& tr = view.get<Transform>(*it2);

            if (collision.collidesWith[(int)col.entityType])
            {
                // Perform the collision check
                CollisionInfo collisionInfo = collision.GetCollisionInfo(transform.position, col, tr.position);

                if (collisionInfo.hasCollided)
                {
                    if (collision.entityType == PlatformerCollisionID::PLAYER)                         //
                    {                                                                                  //These are placeholders for now
                        if (col.entityType == PlatformerCollisionID::TERRAIN ||                        //
                            col.entityType == PlatformerCollisionID::PLAYER)                           //
                        {
                            collision.listOfEntities.push_back(*it2);
                            collision.listOfCollisionSides.push_back(collisionInfo.collisionSide);

                            col.listOfEntities.push_back(*it);
                            col.listOfCollisionSides.push_back(collisionInfo.collisionSideOther);
                        }
                    }

                }
            }
            it2++;
        }
        LimitPlayerMovement(character, collision);//goes through all the collisions and limits the movement of the player depending on said collisions
        it++;
    }

}

CollisionInfo PlatformerCollision::GetCollisionInfo(Vector3& pos_, const PlatformerCollision& other_, Vector3& posOther_)
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
                if (state == MovementState::MOVEABLE) pos_.x -= p.x + size.x - p2.x + 0.1f;

                collisionInfo.collisionSideOther = CollisionSide::LEFT;
                if (other_.state == MovementState::MOVEABLE) posOther_.x -= p2.x - p.x - size.x - 0.1f;
            }
        }


        if (p.x <= (p2.x + other_.size.x) && p.x >= p2.x)
        {
            if ((abs(p.x - p2.x - other_.size.x) < abs(p.y - p2.y - other_.size.y)) && ((abs(p.x - p2.x - other_.size.x) < abs(p.y + size.y - p2.y))))
            {
                // bumped into other_ from the right side
                collisionInfo.collisionSide = CollisionSide::LEFT;
                if (state == MovementState::MOVEABLE) pos_.x -= p.x - p2.x - other_.size.x - 0.1f;

                collisionInfo.collisionSideOther = CollisionSide::RIGHT;
                if (other_.state == MovementState::MOVEABLE) posOther_.x -= p2.x + other_.size.x - p.x + 0.1f;
            }
        }


        if ((p.y + size.y) >= p2.y && (p2.y + other_.size.y) >= (p.y + size.y))
        {
            if ((abs(p.y + size.y - p2.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y + size.y - p2.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the bottom side
                collisionInfo.collisionSide = CollisionSide::TOP;

                collisionInfo.collisionSideOther = CollisionSide::BOTTOM;
                /*if (other_.state == MovementState::MOVEABLE)*/ posOther_.y -= p2.y - p.y - size.y - 0.1f;
            }
        }

        if (p.y <= (p2.y + other_.size.y) && p.y >= p2.y)
        {
            if ((abs(p.y - p2.y - other_.size.y) < abs(p.x - p2.x - other_.size.x)) && ((abs(p.y - p2.y - other_.size.y) < abs(p.x + size.x - p2.x))))
            {
                // bumped into other_ from the top side
                collisionInfo.collisionSide = CollisionSide::BOTTOM;
                /*if (other_.state == MovementState::MOVEABLE)*/ pos_.y -= p.y - p2.y - other_.size.y - 0.1f;

                collisionInfo.collisionSideOther = CollisionSide::TOP;
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

void PlatformerCollisionSystem::LimitPlayerMovement(PlatformerCharacter& character_, PlatformerCollision collision_)
{
    if (!collision_.listOfEntities.empty())
    {
        for (int i = 0; i < collision_.listOfEntities.size(); i++)
        {
            character_.canGoRight = (collision_.listOfCollisionSides[i] == CollisionSide::RIGHT) ? false : character_.canGoRight;//character_.canGoRight;
            character_.canGoLeft = (collision_.listOfCollisionSides[i] == CollisionSide::LEFT) ? false : character_.canGoLeft; //character_.canGoLeft;
            character_.canGoUp = (collision_.listOfCollisionSides[i] == CollisionSide::TOP) ? false : character_.canGoUp; //character_.canGoUp;
            character_.canGoDown = (collision_.listOfCollisionSides[i] == CollisionSide::BOTTOM) ? false : character_.canGoDown; //character_.canGoDown;
        }
    }
}
