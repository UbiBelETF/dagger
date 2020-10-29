#include "simple_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/ping_pong/pingpong_ball.h"

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

            if (collision.IsCollided(transform.position, col, tr.position))
            {
                // TODO: remove PingPongBall usage
                if (Engine::Registry().has<pingPong::PingPongBall>(*it))
                {
                    pingPong::PingPongBall& ball = Engine::Registry().get<pingPong::PingPongBall>(*it);
                    // one collision per frame per component
                    if (!ball.collided)
                    {
                        ball.collisionSides = collision.GetCollisionSides(transform.position, col, tr.position);
                        ball.collided = true;
                    }
                }

                if (Engine::Registry().has<pingPong::PingPongBall>(*it2))
                {
                    pingPong::PingPongBall& ball = Engine::Registry().get<pingPong::PingPongBall>(*it2);
                    // one collision per frame per component
                    if (!ball.collided)
                    {
                        ball.collisionSides = col.GetCollisionSides(tr.position, collision, transform.position);
                        ball.collided = true;
                    }
                }
            }
            it2++;
        }
        it++;
    }
}