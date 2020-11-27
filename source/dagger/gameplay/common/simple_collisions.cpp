#include "simple_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include <list>
#include <math.h>

using namespace dagger;

void SimpleCollisionsSystem::Run()
{
    auto view = Engine::Registry().view<SimpleCollision, Transform>();

    auto it = view.begin();
    while (it != view.end())
    {
        auto &collision = view.get<SimpleCollision>(*it);
        collision.colided = false;
        collision.colisions.clear();
    }

    it = view.begin();
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
                collision.colisions.emplace_back(*it2);

                col.colided = true;
                col.colidedWith = *it;
                col.colisions.emplace_back(*it);
            }
            it2++;
        }
        it++;
    }
}

// SimpleCollision

bool SimpleCollision::IsCollided(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_) // A bit more complicated now circles are in play
{
    Float32 maxDistanceFromCenters = 0; // Sum of max distances from the center to the egde of the object; if the object is circular that max distance 
                                        // equals the radius of the circle; if rectangular, it equals half the lenght of the diagonal (distance to the corner)
    // Info about this
    Vector2 p1;
    Vector2 p1Size;
    p1.x = pos_.x + pivot.x * size.x;
    p1Size.x = size.x;
    if (shape == EHitbox::Rectangular) {
        p1.y = pos_.y + pivot.y * size.y;
        p1Size.y = size.y;
        
        maxDistanceFromCenters += sqrt(pow(size.x / 2.0f, 2) + pow(size.y / 2.0f, 2));
    }
    else {
        p1.y = pos_.y + pivot.x * size.x; // size.x is the diameter
        p1Size.y = size.x;

        maxDistanceFromCenters += size.x / 2.0f;
    }

    // Info about other
    Vector2 p2;
    Vector2 p2Size;
    p2.x = posOther_.x + other_.pivot.x * other_.size.x;
    p2Size.x = other_.size.x;
    if (other_.shape == EHitbox::Rectangular) {
        p2.y = posOther_.y + other_.pivot.y * other_.size.y;
        p2Size.y = other_.size.y;

        maxDistanceFromCenters += sqrt(pow(other_.size.x / 2.0f, 2) + pow(other_.size.y / 2.0f, 2));
    }
    else {
        p2.y = posOther_.y + other_.pivot.x * other_.size.x; // size.x is the diameter
        p2Size.y = other_.size.x;

        maxDistanceFromCenters += other_.size.x / 2.0f;
    }

    Float32 distanceFromCenters = sqrt(pow(posOther_.x - pos_.x, 2) + pow(posOther_.y - pos_.y, 2));

    if (distanceFromCenters > maxDistanceFromCenters) return false; // If the current distance is greater than the max distance at which the collision 
    else {                                                          // happens, collision won't happen regardless of the object positioning
        if (p1.x < p2.x + p2Size.x &&                               // This allows for accurate aproximation of the circle with a rectangle
            p1.x + p1Size.x > p2.x &&                               // because the cases where a rectangle would colide with an object but  
            p1.y < p2.y + p2Size.y &&                               // a circle wouldn't are excluded because distance is far too great for 
            p1.y + p1Size.y > p2.y)                                 // collision to happen
        {
            return true;
        }

        return false;
    }
}

Vector2 SimpleCollision::GetCollisionSides(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
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

Vector3 SimpleCollision::GetCollisionCenter(const Vector3& pos_, const SimpleCollision& other_, const Vector3& posOther_)
{
    Vector3 res;

    Vector2 p(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    res.x = (p.x + size.x / 2.f + p2.x + other_.size.x) / 2.f;
    res.y = (p.y + size.y / 2.f + p2.y + other_.size.y) / 2.f;
    res.z = 1.f;

    return res;
}
