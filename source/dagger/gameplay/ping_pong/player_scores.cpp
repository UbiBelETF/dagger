#include "player_scores.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/simple_collisions.h"
#include "gameplay/ping_pong/pingpong_ball.h"

#include "gameplay/ping_pong/ping_pong_main.h"

using namespace pingPong;

int PlayerScoresSystem::FIELD_WIDTH = 20;
int PlayerScoresSystem::FIELD_HEIGHT = 10;
Float32 PlayerScoresSystem::TILE_SIZE = 1.f;

void PlayerScoresSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform>();

    int ballOnField = 0;

    for (auto entity : view)
    {
        auto &ball = view.get<PingPongBall>(entity);
        auto &t = view.get<Transform>(entity);

        if (ball.reachedGoal && !ball.processed)
        {
            ball.speed = { 0,0,0 };

            if (ball.playerOneScored)
            {
                m_goalsPlayerOne++;
                t.position = { (-(FIELD_WIDTH+3) / 2.f) * TILE_SIZE,  TILE_SIZE * m_goalsPlayerOne, 0 };
            }
            else
            {
                m_goalsPlayerTwo++;
                t.position = { ((FIELD_WIDTH+3) / 2.f) * TILE_SIZE, TILE_SIZE * m_goalsPlayerTwo, 0 };
            }

            ball.processed = true;
        }

        if (!ball.reachedGoal)
        {
            ballOnField++;
        }
    }

    if (ballOnField == 0)
    {
        auto& reg = Engine::Registry();
        pingPong::CreatePingPongBall(reg, TILE_SIZE, Color(1, 1, 1, 1), { rand()%10 + 4,rand()%10 + 4,0 },   { 0,rand()%(FIELD_HEIGHT / 2),0 });
    }
}
