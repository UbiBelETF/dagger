#include "plight_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace plight;

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
            if (collision.IsCollidedSAT(transform.position, col, tr.position))
            {
                collision.colided = true;
                collision.colidedWith.push_back(*it2);

                col.colided = true;
                col.colidedWith.push_back(*it);
            }
            it2++;
        }

        collision.last_pos = transform;

        it++;
    }
}


void plight::PlightCollision::Setup()
{
    
    auto& transform = view.get<Transform>(*it);
    last_pos = transform.position;
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


bool PlightCollision::IsCollidedSAT(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_)
{
    // The polygons
    Vector2 p1(pos_.x + pivot.x * size.x, pos_.y + pivot.y * size.y);
    Vector2 p2(posOther_.x + other_.pivot.x * other_.size.x, posOther_.y + other_.pivot.y * other_.size.y);

    // The points of the polygons
    Vector2 p1_points[4] = { Vector2(p1.x, p1.y), Vector2(p1.x + size.x, p1.y) , Vector2(p1.x + size.x, p1.y + size.y) , Vector2(p1.x, p1.y + size.y) };
    Vector2 p2_points[4] = { Vector2(p2.x, p2.y), Vector2(p2.x + other_.size.x, p2.y) , Vector2(p2.x + other_.size.x, p2.y + other_.size.y) , Vector2(p2.x, p2.y + other_.size.y) };

    // currently hardcoded to work only with rectangles, but could be extended to any convex polygons
    for (auto i = 0u; i < 2; ++i) {
        // calculate the normal vector of the current edge
        // this is the axis will we check in this loop
        Vector2 normal(p1_points[i+1] - p1_points[i]);

        // loop over all vertices of both polygons and project them
        // onto the axis. We are only interested in max/min projections
        auto aMaxProj = -std::numeric_limits<float>::infinity();
        auto aMinProj = std::numeric_limits<float>::infinity();
        auto bMaxProj = -std::numeric_limits<float>::infinity();
        auto bMinProj = std::numeric_limits<float>::infinity();
        for (auto i = 0u; i < 4; ++i) {
            auto proj = glm::dot(normal, p1_points[i]);
            if (proj < aMinProj) aMinProj = proj;
            if (proj > aMaxProj) aMaxProj = proj;
        }

        for (auto i = 0u; i < 4; ++i) {
            auto proj = glm::dot(normal, p2_points[i]);
            if (proj < bMinProj) bMinProj = proj;
            if (proj > bMaxProj) bMaxProj = proj;
        }

        // now check if the intervals the both polygons projected on the
        // axis overlap. If they don't, we have found an axis of separation and
        // the given polygons cannot overlap
        if (bMinProj > aMaxProj  || aMinProj > bMaxProj) {
            return false;
        }

        Vector2 normal2(p2_points[i + 1] - p2_points[i]);
        if (normal2 != normal) {

            // loop over all vertices of both polygons and project them
            // onto the axis. We are only interested in max/min projections
            auto aMaxProj = -std::numeric_limits<float>::infinity();
            auto aMinProj = std::numeric_limits<float>::infinity();
            auto bMaxProj = -std::numeric_limits<float>::infinity();
            auto bMinProj = std::numeric_limits<float>::infinity();
            for (auto i = 0u; i < 4; ++i) {
                auto proj = glm::dot(normal2, p1_points[i]);
                if (proj < aMinProj) aMinProj = proj;
                if (proj > aMaxProj) aMaxProj = proj;
            }

            for (auto i = 0u; i < 4; ++i) {
                auto proj = glm::dot(normal2, p2_points[i]);
                if (proj < bMinProj) bMinProj = proj;
                if (proj > bMaxProj) bMaxProj = proj;
            }

            if (bMinProj > aMaxProj || aMinProj > bMaxProj) {
                return false;
            }
        }
    }

    // at this point, we have checked all axis but found no separating axis
    // which means that the polygons must intersect.

    place = last_pos.position;
    return true;
}

bool PlightCollision::IsCollidedSweptSAT(const Vector3& pos_, const PlightCollision& other_, const Vector3& posOther_)
{
    // The points of the polygons
    int len1, len2;
    Vector2* p1_points = GetSweptObject(len1, *this, pos_);
    Vector2* p2_points = GetSweptObject(len2, other_, posOther_);

    for (auto i = 0u; i < len1/2; ++i) {
        // calculate the normal vector of the current edge
        // this is the axis will we check in this loop
        Vector2 normal(p1_points[i + 1] - p1_points[i]);

        // loop over all vertices of both polygons and project them
        // onto the axis. We are only interested in max/min projections
        auto aMaxProj = -std::numeric_limits<float>::infinity();
        auto aMinProj = std::numeric_limits<float>::infinity();
        auto bMaxProj = -std::numeric_limits<float>::infinity();
        auto bMinProj = std::numeric_limits<float>::infinity();
        for (auto i = 0u; i < len1; ++i) {
            auto proj = glm::dot(normal, p1_points[i]);
            if (proj < aMinProj) aMinProj = proj;
            if (proj > aMaxProj) aMaxProj = proj;
        }

        for (auto i = 0u; i < len2; ++i) {
            auto proj = glm::dot(normal, p2_points[i]);
            if (proj < bMinProj) bMinProj = proj;
            if (proj > bMaxProj) bMaxProj = proj;
        }

        // now check if the intervals the both polygons projected on the
        // axis overlap. If they don't, we have found an axis of separation and
        // the given polygons cannot overlap
        if (bMinProj > aMaxProj || aMinProj > bMaxProj) {
            delete p1_points;
            delete p2_points;
            return false;
        }
    }

    for (auto i = 0u; i < len2 / 2; ++i) {
        // calculate the normal vector of the current edge
        // this is the axis will we check in this loop
        Vector2 normal(p2_points[i + 1] - p2_points[i]);

        // loop over all vertices of both polygons and project them
        // onto the axis. We are only interested in max/min projections
        auto aMaxProj = -std::numeric_limits<float>::infinity();
        auto aMinProj = std::numeric_limits<float>::infinity();
        auto bMaxProj = -std::numeric_limits<float>::infinity();
        auto bMinProj = std::numeric_limits<float>::infinity();
        for (auto i = 0u; i < len2; ++i) {
            auto proj = glm::dot(normal, p2_points[i]);
            if (proj < aMinProj) aMinProj = proj;
            if (proj > aMaxProj) aMaxProj = proj;
        }

        for (auto i = 0u; i < len1; ++i) {
            auto proj = glm::dot(normal, p1_points[i]);
            if (proj < bMinProj) bMinProj = proj;
            if (proj > bMaxProj) bMaxProj = proj;
        }

        // now check if the intervals the both polygons projected on the
        // axis overlap. If they don't, we have found an axis of separation and
        // the given polygons cannot overlap
        if (bMinProj > aMaxProj || aMinProj > bMaxProj) {
            delete p1_points;
            delete p2_points;
            return false;
        }
    }

    // at this point, we have checked all axis but found no separating axis
    // which means that the polygons must intersect.
    delete p1_points;
    delete p2_points;
    place = last_pos.position;
    return true;
}

Vector2* PlightCollision::GetSweptObject(int& len_, const PlightCollision& col_, const Vector3& pos_) {
    Vector2 p1(pos_.x + col_.pivot.x * col_.size.x, pos_.y + col_.pivot.y * col_.size.y);
    Vector2 p2(col_.last_pos.position.x + col_.pivot.x * col_.size.x, col_.last_pos.position.y + col_.pivot.y * col_.size.y);

    if (p1.x < p2.x) {
        len_ = 6;
        if (p1.y < p2.y) {
            Vector2* ret = new Vector2[6]{ Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), Vector2(p2.x + col_.size.x, p2.y), Vector2(p2.x + col_.size.x, p2.y + col_.size.y), Vector2(p1.x + col_.size.x, p1.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
        else if (p1.y > p2.y) {
            Vector2* ret = new Vector2[6]{ Vector2(p1.x, p1.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p2.x + col_.size.x, p2.y) , Vector2(p2.x + col_.size.x, p2.y + col_.size.y) , Vector2(p2.x, p2.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
        else {
            len_ = 4;
            Vector2* ret = new Vector2[4]{ Vector2(p1.x, p1.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p2.x + col_.size.x, p2.y + col_.size.y) , Vector2(p2.x, p2.y + col_.size.y) };
            return ret;
        }
    }
    else if (p1.x > p2.x) {
        len_ = 6;
        if (p1.y < p2.y) {
            Vector2* ret = new Vector2[6]{ Vector2(p2.x, p2.y), Vector2(p2.x + col_.size.x, p2.y), Vector2(p2.x, p2.y + col_.size.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p1.x + col_.size.x, p1.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
        else if (p1.y > p2.y) {
            Vector2* ret = new Vector2[6]{ Vector2(p1.x, p1.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p1.x + col_.size.x, p1.y + col_.size.y) ,  Vector2(p2.x, p2.y), Vector2(p2.x + col_.size.x, p2.y + col_.size.y) , Vector2(p2.x, p2.y + col_.size.y) };
            return ret;
        }
        else {
            len_ = 4;
            Vector2* ret = new Vector2[4]{ Vector2(p2.x, p2.y), Vector2(p2.x + col_.size.x, p2.y) , Vector2(p1.x + col_.size.x, p1.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
    }
    else {
        len_ = 4;
        if (p1.y < p2.y) {
            Vector2* ret = new Vector2[4]{ Vector2(p1.x, p1.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p2.x + col_.size.x, p2.y + col_.size.y) , Vector2(p2.x, p2.y + col_.size.y) };
            return ret;
        }
        else if (p1.y > p2.y) {
            Vector2* ret = new Vector2[4]{ Vector2(p2.x, p2.y), Vector2(p2.x + col_.size.x, p2.y) , Vector2(p1.x + col_.size.x, p1.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
        else {
            Vector2* ret = new Vector2[4]{ Vector2(p1.x, p1.y), Vector2(p1.x + col_.size.x, p1.y) , Vector2(p1.x + col_.size.x, p1.y + col_.size.y) , Vector2(p1.x, p1.y + col_.size.y) };
            return ret;
        }
    }
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