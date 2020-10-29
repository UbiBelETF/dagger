#include "simple_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;

void SimpleCollisionsSystem::Run()
{
    auto view = Engine::Registry().view<SimpleCollision, Transform>();

    auto it = view.begin();
    while(it != view.end())
    {
        auto &collision = view.get<SimpleCollision>(*it);
        auto &transform = view.get<Transform>(*it);

        auto it2 = it;
        it2++;
        while(it2 != view.end())
        {
            auto &col = view.get<SimpleCollision>(*it2);
            auto &tr = view.get<Transform>(*it2);

            // processing one collision per frame for each colider
            if (collision.IsCollided(transform.position, col, tr.position))
            {
                collision.colided = true;
                collision.colidedWith = *it2;

                col.colided = true;
                col.colidedWith = *it;
            }
            it2++;
        }
        it++;
    }
}

// SimpleCollision

bool SimpleCollision::IsCollided(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther)
{
    Vector2 p(pos.x + pivot.x * size.x, pos.y + pivot.y * size.y);
    Vector2 p2(posOther.x + other.pivot.x * other.size.x, posOther.y + other.pivot.y * other.size.y);

    if (p.x < p2.x + other.size.x &&
        p.x + size.x > p2.x &&
        p.y < p2.y + other.size.y &&
        p.y + size.y > p2.y)
    {
        return true;
    }

    return false;
}

Vector2 SimpleCollision::GetCollisionSides(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther)
{
    Vector2 res(0, 0);

    Vector2 p(pos.x + pivot.x * size.x, pos.y + pivot.y * size.y);
    Vector2 p2(posOther.x + other.pivot.x * other.size.x, posOther.y + other.pivot.y * other.size.y);

    Float32 xDif = std::min(std::abs(p.x + size.x - p2.x), std::abs(p2.x + other.size.x - p.x));
    Float32 yDif = std::min(std::abs(p.y + size.y - p2.y), std::abs(p2.y + other.size.y - p.y));

    if (xDif < yDif)
    {
        res.x = std::abs(p.x + size.x - p2.x) < std::abs(p2.x + other.size.x - p.x) ? 1 : -1;
    }
    else
    {
        res.y = std::abs(p.y + size.y - p2.y) < std::abs(p2.y + other.size.y - p.y) ? 1 : -1;
    }

    return res;
}

Vector3 SimpleCollision::GetCollisionCenter(const Vector3& pos, const SimpleCollision& other, const Vector3& posOther)
{
    Vector3 res;

    Vector2 p(pos.x + pivot.x * size.x, pos.y + pivot.y * size.y);
    Vector2 p2(posOther.x + other.pivot.x * other.size.x, posOther.y + other.pivot.y * other.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other.size.y) / 2.f;
    res.z = 1.f;

    return res;
}
