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
    auto viewCollisions = Engine::Registry().view<Physics,Transform, SimpleCollision>();
    auto view = Engine::Registry().view< Physics, Transform, SimpleCollision>();
    
    
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& col = view.get<SimpleCollision>(entity);
        auto& physics = view.get<Physics>(entity);
        if (physics.nonStatic) {
            for (auto entity2 : viewCollisions)
            {
                if (entity2 != entity)
                {
                    Vector3 temp_t = { 0,0,0 };
                    temp_t.x = t.position.x + physics.velocity.x * Engine::DeltaTime();
                    temp_t.y = t.position.y + physics.velocity.y * Engine::DeltaTime();
                    temp_t.z = 0;
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(entity2);
                    Transform& transform = viewCollisions.get<Transform>(entity2);
                    Physics& phy_ = viewCollisions.get<Physics>(entity2);
                    //THIS IS THE PASSING THROUGH NONSTATIC OBJECT OPTION
                    /*if (col.IsCollided(temp_t, collision, transform.position) && !phy_.nonStatic)
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
                            if (collisionSides.y != 0)
                            {
                                if (physics.velocity.x != 0 && physics.velocity.y != GetGravity() * dt) {
                                    temp_t.x -= physics.velocity.x * dt; physics.velocity.x = 0;
                                }
                                else
                                    temp_t.y -= physics.velocity.y * dt;

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

                    }*/
                    if (col.IsCollided(temp_t, collision, transform.position) )
                    {
                        UInt32 cnt = 0;
                        Vector2 collisionSides = col.GetCollisionSides(temp_t, collision, transform.position);
                        Float32 dt = Engine::DeltaTime();
                        do
                        {
                             collisionSides = col.GetCollisionSides(temp_t, collision, transform.position);
                            // get back for 1 frame 
                            if (std::abs(collisionSides.x) > 0)
                            {
                                if (phy_.nonStatic) {
                                    if (physics.velocity.x == 0)break;
                                    if (physics.velocity.x > 0 && phy_.velocity.x > 0 && collisionSides.x > 0) break;
                                    if (physics.velocity.x < 0 && phy_.velocity.x < 0 && collisionSides.x < 0) break;
                                    else temp_t.x -= physics.velocity.x * dt;
                                }
                                else
                                {
                                    if (collisionSides.x == 1)
                                    {
                                        cnt++;
                                        if (physics.velocity.x > 0)
                                         temp_t.x -= physics.velocity.x * dt; 
                                        else temp_t.y -= physics.velocity.y * dt;
                                    }
                                    else
                                    {
                                        if (physics.velocity.x < 0) temp_t.x -= physics.velocity.x * dt;
                                        else temp_t.y -= physics.velocity.y * dt;
                                    }
                                }
                            }
                            else 
                            {
                                if (phy_.nonStatic) {
                                    if (physics.velocity.x == 0)break;
                                    if (physics.velocity.x > 0 && phy_.velocity.x > 0 && collisionSides.x > 0) break;
                                    if (physics.velocity.x < 0 && phy_.velocity.x < 0 && collisionSides.x < 0) break;
                                    else temp_t.x -= physics.velocity.x * dt;
                                }
                                else {
                                    if (collisionSides.y == 1)
                                    {
                                        if (physics.velocity.y > 0) temp_t.y -= physics.velocity.y * dt;
                                        else temp_t.x -= physics.velocity.x * dt;
                                    }
                                    else
                                    {
                                        if (physics.velocity.y < 0) temp_t.y -= physics.velocity.y * dt;
                                        else temp_t.x -= physics.velocity.x * dt;
                                        auto& b = Engine::Registry().get<BrawlerCharacter>(entity);
                                        b.doubleJump = false;
                                        b.jump = false;
                                    }
                                }
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

    
}
