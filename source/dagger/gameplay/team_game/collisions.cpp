#include "collisions.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/physics.h"
#include <algorithm>
#include <execution>


void team_game::CollisionSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
    auto view = Engine::Registry().view< Physics, Transform, SimpleCollision>();
    
    
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto& physics = view.get<Physics>(entity);
        UINT32 attempts = 0;
        /*if (col.colided) {
            if (Engine::Registry().valid(col.colidedWith))
            {
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
                Transform& transform = viewCollisions.get<Transform>(col.colidedWith);
                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                do
                {
                    // get back for 1 frame
                    Float32 dt = Engine::DeltaTime();
                    if (std::abs(collisionSides.x) > 0)
                    {
                        t.position.x -= (physics.velocity.x * dt);
                    }

                    if (std::abs(collisionSides.y) > 0)
                    {
                        t.position.y -= (physics.velocity.y * dt);
                    }
                } while (col.IsCollided(t.position, collision, transform.position));
                if (std::abs(collisionSides.x) > 0)
                {
                    physics.velocity.x =0;
                }

                if (std::abs(collisionSides.y) > 0)
                {
                    physics.velocity.y = 0;
                }
            }
            col.colided = false;
        }*/
        for (auto entity2 : viewCollisions)
        {
            if (entity2 != entity)
            {
                Vector3 temp_t = { 0,0,0 };
                temp_t.x = t.position.x + physics.velocity.x * Engine::DeltaTime();
                temp_t.y = t.position.y + physics.velocity.y * Engine::DeltaTime();
                temp_t.z = 1;
                SimpleCollision& collision = viewCollisions.get<SimpleCollision>(entity2);
                Transform& transform = viewCollisions.get<Transform>(entity2);
                if (col.IsCollided(temp_t, collision, transform.position))
                {

                    Vector2 collisionSides = col.GetCollisionSides(temp_t, collision, transform.position);

                    do
                    {

                        // get back for 1 frame 
                        Float32 dt = Engine::DeltaTime();
                        if (std::abs(collisionSides.x) > 0)
                        {
                            temp_t.x -= (physics.velocity.x * dt);
                        }

                        if (std::abs(collisionSides.y) > 0)
                        {
                            temp_t.y -= (physics.velocity.y * dt);
                        }


                    } while (col.IsCollided(temp_t, collision, transform.position));
                    if (std::abs(collisionSides.x) > 0)
                    {
                        physics.velocity.x = (temp_t.x - t.position.x) / Engine::DeltaTime();
                    }

                    if (std::abs(collisionSides.y) > 0)
                    {
                        physics.velocity.y = (temp_t.y - t.position.y) / Engine::DeltaTime();
                    }
                    
                }
            }
        }
       
    }

    
}
