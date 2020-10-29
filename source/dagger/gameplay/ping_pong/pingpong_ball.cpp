#include "pingpong_ball.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/simple_collisions.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace pingPong;

void PingPongBallSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform, SimpleCollision>();
    for(auto entity: view) 
    {
        auto &t = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);
        auto &col = view.get<SimpleCollision>(entity);

        if (ball.reachedGoal)
        {
            continue;
        }

        if (col.colided)
        {
            if (Engine::Registry().has<SimpleCollision>(col.colidedWith) 
                && Engine::Registry().has<Transform>(col.colidedWith))
            {
                SimpleCollision& collision = Engine::Registry().get<SimpleCollision>(col.colidedWith);
                Transform& transform = Engine::Registry().get<Transform>(col.colidedWith);

                Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

                do
                {
                    // get back for 1 frame 
                    Float32 dt = Engine::DeltaTime();
                    if (std::abs(collisionSides.x) > 0)
                    {
                        t.position.x -= (ball.speed.x * dt);
                    }

                    if (std::abs(collisionSides.y) > 0)
                    {
                        t.position.y -= (ball.speed.y * dt);
                    }
                        
                } while (col.IsCollided(t.position, collision, transform.position));

                if (std::abs(collisionSides.x) > 0)
                {
                    ball.speed.x *= -1;
                }

                if (std::abs(collisionSides.y) > 0)
                {
                    ball.speed.y *= -1;
                }
            }

            if (Engine::Registry().has<PingPongWall>(col.colidedWith))
            {
                PingPongWall& wall = Engine::Registry().get<PingPongWall>(col.colidedWith);
                ball.reachedGoal = true;
                ball.playerOneScored = !wall.isLeft;
            }

            col.colided = false;
        }
        else
        {
            t.position += (ball.speed * Engine::DeltaTime());
        }
    }
}
