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
       if (col.colided)
        {
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
                        t.position.x -= physics.velocity.x*dt;
                    }

                    if (std::abs(collisionSides.y) > 0)
                    {
                        t.position.y -= physics.velocity.y*dt;
                    }

                } while (col.IsCollided(t.position, collision, transform.position));
                
                Vector3 temp_t = { 0,0,0 };
                temp_t.x = t.position.x + physics.velocity.x * Engine::DeltaTime();
                temp_t.y = t.position.y + physics.velocity.y * Engine::DeltaTime();
                temp_t.z = t.position.z;
                if (col.IsCollided(temp_t, collision, transform.position)) {
                    while (col.IsCollided(temp_t, collision, transform.position))
                    {
                        // get back for 1 frame 
                        Float32 dt = Engine::DeltaTime();
                        if (std::abs(collisionSides.x) > 0)
                        {
                            temp_t.x -= physics.velocity.x * dt;
                        }

                        if (std::abs(collisionSides.y) > 0)
                        {
                            temp_t.y -= physics.velocity.y * dt;
                        }

                    }
                    physics.velocity.x = (temp_t.x - t.position.x)/Engine::DeltaTime();
                    physics.velocity.y = (temp_t.x - t.position.x) / Engine::DeltaTime();
                    if (physics.velocity.y == 0) exit(0);
                }
            }
        }
    }
}
