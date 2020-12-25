#include "plight_physics.h"
#include "plight_collisions.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_aiming.h"
#include "gameplay/plight/plight_fields.h"

#include <algorithm>    

using namespace plight;
using namespace dagger;

void plight::PhysicsSystem::Run()
{
    auto view = Engine::Registry().view<PlightCollision, Transform, PhysicsObject>();
    auto it = view.begin();


    while (it != view.end())
    {
        auto& physics = view.get<PhysicsObject>(*it);
        if (!physics.is_static) {
            auto& collision = view.get<PlightCollision>(*it);
            auto& transform = view.get<Transform>(*it);

            auto it2 = collision.colidedWith.begin();
            while (it2 != collision.colidedWith.end())
            {

                bool skip = false;
                if (Engine::Registry().has<PhysicsObject>(*it2)) {
                    auto& other_physics = view.get<PhysicsObject>(*it2);
                    bool has = false;
                    for (int group : other_physics.collision_groups) {
                        if (std::find(physics.my_groups.begin(), physics.my_groups.end(), group) != physics.my_groups.end()) {
                            has = true;
                            break;
                        }
                    }
                    if (Engine::Registry().has<DefenseField>(*it)) {
                        auto& def_field = Engine::Registry().get<DefenseField>(*it);
                        if (*it2 == def_field.defenseFieldE) {
                            has = false;
                        }
                    }
                    if (has) {
                        auto& other_transform = view.get<Transform>(*it2);
                        auto& other_collision = view.get<PlightCollision>(*it2);
                        if (other_physics.is_static)
                            ResolveCollision(transform.position, collision, *it, other_transform.position, other_collision, it2);
                        else 
                            ResolveDynamicCollision(transform.position, collision, *it, other_transform.position, other_collision, it2);
                        skip = true;
                    }
                }
                if (!skip) it2++;
            }
        }
        it++;
    }
}

void plight::PhysicsSystem::ResolveCollision(Vector3& pos_, PlightCollision& myCol_, entt::entity my_, const Vector3& posOther_, PlightCollision& otherCol_, std::list<entt::entity>::iterator& other_)
{
    // Moves the coliding object out of the other coliding object & increments it2
   Vector2 sides = myCol_.GetCollisionSides(pos_, otherCol_, posOther_);

    if (sides.x != 0) {
        if (sides.x == 1) pos_.x = posOther_.x + otherCol_.pivot.x * otherCol_.size.x - (1+ myCol_.pivot.x) * myCol_.size.x;
        else pos_.x = posOther_.x + otherCol_.pivot.x * otherCol_.size.x + otherCol_.size.x - myCol_.pivot.x * myCol_.size.x;
    }
    if (sides.y != 0) {
        if (sides.y == 1) pos_.y = posOther_.y + otherCol_.pivot.y * otherCol_.size.y - (1 + myCol_.pivot.y) * myCol_.size.y;
        else pos_.y = posOther_.y + otherCol_.pivot.y * otherCol_.size.y + otherCol_.size.y - myCol_.pivot.y * myCol_.size.y;
    }

    other_=myCol_.colidedWith.erase(other_);
    if(myCol_.colidedWith.size() == 0) myCol_.colided = false;

    otherCol_.colidedWith.remove(my_);
    if (otherCol_.colidedWith.size() == 0) otherCol_.colided = false;
}

void plight::PhysicsSystem::ResolveDynamicCollision(Vector3& pos_, PlightCollision& myCol_, entt::entity my_, Vector3& posOther_, PlightCollision& otherCol_, std::list<entt::entity>::iterator& other_)
{
    // Moves the coliding object out of the other coliding object & increments it2
    Vector2 sides = myCol_.GetCollisionSides(pos_, otherCol_, posOther_);
    float diference;
    if (sides.x != 0) {
        
        if (sides.x == 1) {
            diference = pos_.x + myCol_.size.x - posOther_.x;
            pos_.x -= diference /2;
            posOther_.x += diference / 2;
        }
        else {
            diference = posOther_.x + otherCol_.size.x - pos_.x;
            pos_.x += diference/2;
            posOther_.x -= diference / 2;
        }
    }
    if (sides.y != 0) {
        if (sides.y == 1) {
            diference = pos_.y + myCol_.size.y - posOther_.y;
            pos_.y -= diference /2;
            posOther_.y += diference / 2;
        }
        else {
            diference = posOther_.y + otherCol_.size.y - pos_.y;
            pos_.y += diference /2;
            posOther_.y -= diference / 2;
        }
    }

    other_ = myCol_.colidedWith.erase(other_);
    if (myCol_.colidedWith.size() == 0) myCol_.colided = false;

    otherCol_.colidedWith.remove(my_);
    if (otherCol_.colidedWith.size() == 0) otherCol_.colided = false;
}
