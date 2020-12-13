#include "plight_physics.h"
#include "plight_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include <algorithm>    

using namespace plight;
using namespace dagger;

void plight::PhysicsSystem::Run()
{
    auto view = Engine::Registry().view<PlightCollision, Transform, PhysicsObject>();
    auto it = view.begin();

    auto& physics = view.get<PhysicsObject>(*it);
    if (!physics.is_static) {
        while (it != view.end())
        {
            auto& collision = view.get<PlightCollision>(*it);
            auto& transform = view.get<Transform>(*it);

            auto it2 = collision.colidedWith.begin();
            while (it2 != collision.colidedWith.end())
            {
                if (Engine::Registry().has<PhysicsObject>(*it2)) {
                    auto& other_physics = view.get<PhysicsObject>(*it2);
                    bool has = false;
                    for (int group : other_physics.collision_groups) {
                        if (std::find(physics.my_groups.begin(), physics.my_groups.end(), group) != physics.my_groups.end()) {
                            has = true;
                            break;
                        }
                    }
                    if (has) {
                        auto& other_transform = view.get<Transform>(*it2);
                        auto& other_collision = view.get<PlightCollision>(*it2);
                        ResolveCollision(transform.position, collision, other_transform.position, other_collision);
                    }
                }
                it2++;
            }
            it++;
        }
    }
}

void plight::PhysicsSystem::ResolveCollision(Vector3& pos_, PlightCollision& my_, const Vector3& posOther_, const PlightCollision& other_)
{
    Vector2 sides = my_.GetCollisionSides(pos_, other_, posOther_);

    if (sides.x != 0) {
        if (sides.x == 1) pos_.x = posOther_.x - my_.size.x;
        else pos_.x = posOther_.x + other_.size.x;
    }
    if (sides.y != 0) {
        if (sides.y == 1) pos_.y = posOther_.y - my_.size.y;
        else pos_.y = posOther_.y + other_.size.y;
    }

    my_.colided = false;
}

