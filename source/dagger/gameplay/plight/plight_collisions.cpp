#include "plight_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;

void PlightCollisionsSystem::Run()
{
    auto view = Engine::Registry().view<PlightCollision, Transform>();
    auto it = view.begin();

    while (it != view.end()) {
        auto& collision = view.get<PlightCollision>(*it);
        collision.colidedWith.clear();
        collision.colided = false;
        it++;
    }
    it = view.begin();


    while (it != view.end())
    {
        auto& collision = view.get<PlightCollision>(*it);
        auto& transform = view.get<Transform>(*it);

        auto it2 = it;
        it2++;
        while (it2 != view.end())
        {
            auto& col = view.get<PlightCollision>(*it2);
            auto& tr = view.get<Transform>(*it2);

            // processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                collision.colided = true;
                collision.colidedWith.push_back(*it2);

                col.colided = true;
                col.colidedWith.push_back(*it);
            }
            it2++;
        }
        it++;
    }
}


bool PlightCollision::IsCollided(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_)
{
    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    if (p.x < p2.x + other_.size.x &&
        p.x + size.x > p2.x &&
        p.y < p2.y + other_.size.y &&
        p.y + size.y > p2.y)
    {
        return true;
    }

    return false;
}

Vector2 PlightCollision::GetCollisionSides(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_)
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

Vector3 PlightCollision::GetCollisionCenter(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}