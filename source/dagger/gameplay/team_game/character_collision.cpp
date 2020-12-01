#include "character_collision.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "team_game_main.h"

#include <cmath>

using namespace dagger;


Entity CharacterCollisionSystem::MainCharacterEntity;

void CharacterCollisionSystem::Run()
{
    auto view = Engine::Registry().view<CharacterCollision, Transform>();

    auto& col = view.get<CharacterCollision>(MainCharacterEntity);
    auto& tr = view.get<Transform>(MainCharacterEntity);

    auto it = view.begin();
    while (it != view.end())
    {
        auto& collision = view.get<CharacterCollision>(*it);
        auto& transform = view.get<Transform>(*it);

        if (*it != MainCharacterEntity && collision.IsCollided(transform.position, col, tr.position))
        {
            while (collision.IsCollided(transform.position, col, tr.position))
            {
                Vector2 colSides = collision.GetCollisionSides(transform.position, col, tr.position);
                tr.position.x += colSides.x / 10;
                tr.position.y += colSides.y / 10;
            }            
        }
        it++;
    }
}

// SimpleCollision

bool CharacterCollision::IsCollided(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_)
{
    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x&&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        return true;
    }

    return false;
}

Vector2 CharacterCollision::GetCollisionSides(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_)
{
    Vector2 res(0, 0);

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other_.size.x - p.x));
    Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other_.size.y - p.y));

    if (xDif < yDif)
    {
        res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other_.size.x - p.x) ? 1 : -1;
    }
    else
    {
        res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other_.size.y - p.y) ? 1 : -1;
    }

    return res;
}

Vector3 CharacterCollision::GetCollisionCenter(const Vector3& pos_, const CharacterCollision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}
