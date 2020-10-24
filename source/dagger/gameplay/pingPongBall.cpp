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
        auto &transform = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);

        // change 13 later
        if (transform.position.x > 13 && ball.speed.x > 0.f || transform.position.x < -13 && ball.speed.x < 0.f)
        {
            ball.speed.x += ((rand() % 3) - 1) * 0.003f;
            ball.speed.x *= -1;
        }
        if (transform.position.y > 10 && ball.speed.y > 0.f || transform.position.y < -10 && ball.speed.y < 0.f)
        {
            ball.speed.y += ((rand() % 3) - 1) * 0.003f;
            ball.speed.y *= -1;
        }

        transform.position += ball.speed;
    }
}