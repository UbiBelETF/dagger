#include "SimpleCollisions.h"

#include "core/engine.h"
#include "gameplay/transform.h"
#include "gameplay/pingPongBall.h"

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
                if (Engine::Registry().has<PingPongBall>(*it))
                {
                    PingPongBall& ball = Engine::Registry().get<PingPongBall>(*it);
                    if (!ball.collided)
                    {
                        ball.pointOfCollision = collision.GetCollisionCenter(transform.position, col, tr.position);
                        ball.collided = true;
                    }
                }

                if (Engine::Registry().has<PingPongBall>(*it2))
                {
                    PingPongBall& ball = Engine::Registry().get<PingPongBall>(*it2);
                    if (!ball.collided)
                    {
                        ball.pointOfCollision = collision.GetCollisionCenter(transform.position, col, tr.position);
                        ball.collided = true;
                    }
                }
            }
            it2++;
        }
        it++;
    }

    for (auto entity : view)
    {
        

        for (auto entity : view)
        {

        }
    }
}