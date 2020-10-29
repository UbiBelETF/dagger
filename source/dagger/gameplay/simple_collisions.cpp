#include "simple_collisions.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/ping_pong/pingPongBall.h"

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
                if (Engine::Registry().has<PingPong::PingPongBall>(*it))
                {
                    PingPong::PingPongBall& ball = Engine::Registry().get<PingPong::PingPongBall>(*it);
                    // one collision per frame per component
                    if (!ball.collided)
                    {
                        ball.collisionSides = collision.GetCollisionSides(transform.position, col, tr.position);
                        ball.collided = true;
                    }
                }

                if (Engine::Registry().has<PingPong::PingPongBall>(*it2))
                {
                    PingPong::PingPongBall& ball = Engine::Registry().get<PingPong::PingPongBall>(*it2);
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