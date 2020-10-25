#include "pingPongBall.h"

#include "core/engine.h"
#include "gameplay/transform.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void PingPongBallSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform>();
    for(auto entity: view) 
    {
        auto &t = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);

        if (ball.collided)
        {
            if (std::abs(t.position.x - ball.pointOfCollision.x) > std::abs(t.position.y - ball.pointOfCollision.y))
            {
                ball.speed.x *= -1;
            }
            else
            {
                ball.speed.y *= -1;
            }

            ball.collided = false;
        }

        // todo: vector3 doesn't have multiplication to float64
        t.position += (ball.speed * (float)Engine::DeltaTime());
    }
}