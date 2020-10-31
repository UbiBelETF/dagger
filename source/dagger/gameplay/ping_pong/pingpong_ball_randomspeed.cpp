#include "pingpong_ball_randomspeed.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/ping_pong/pingpong_ball.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace pingPong;

void PingPongBallRandomSpeedSystem::Run()
{
    m_timer += Engine::DeltaTime();
    if (m_timer < TIME_TO_CHANGE_SPEED)
    {
        return;
    }

    m_timer = 0;

    auto view = Engine::Registry().view<PingPongBall, Transform>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);

        if (ball.reachedGoal)
        {
            continue;
        }

        Float32 speedAdd = 1.f + static_cast<Float32>(rand() % 10 - 3) / 10.f;

        ball.speed.x *= speedAdd;
        ball.speed.y *= speedAdd;
    }
}

