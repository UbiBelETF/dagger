#include "racing_game_logic.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/racing/racing_simple_collision.h"
#include "gameplay/racing/racing_player_car.h"
#include "gameplay/racing/racing_main.h"
#include "gameplay/racing/racing_big_laser.h"

using namespace dagger;
using namespace racing_game;

void RacingCollisionsLogicSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&RacingCollisionsLogicSystem::OnEndOfFrame>(this);
}

void RacingCollisionsLogicSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&RacingCollisionsLogicSystem::OnEndOfFrame>(this);
}

void RacingCollisionsLogicSystem::Run()
{
    RacingGameFieldSettings fieldSettings;
    {
        static Entity entityFieldSettings;
        if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
        {
            fieldSettings = *ptr;
        }

        auto view = Engine::Registry().view<RacingPlayerCar, Transform, RacingSimpleCollision>();
        for (auto entity : view)
        {
            auto &t = view.get<Transform>(entity);
            auto &player = view.get<RacingPlayerCar>(entity);
            auto &col = view.get<RacingSimpleCollision>(entity);
            if (col.colided)
            {
                m_Restart = true;
            }
        }
    }
}

void RacingCollisionsLogicSystem::OnEndOfFrame()
{
    if (m_Restart)//1st restart condition
    {
        m_Restart = false;
        Engine::Registry().clear();

        racing_game::SetupWorld(Engine::Instance());
    }

    auto view = Engine::Registry().view<RacingGameStats>();//2nd restart condition
    Float32 deltaTime = Engine::DeltaTime();
    for (auto entity : view)
    {
        auto& gameStats = Engine::Registry().get<RacingGameStats>(entity);
        if (gameStats.scores <= 0 && gameStats.resetTimer <= 0)
        {
            m_Restart = false;
            Engine::Registry().clear();

            racing_game::SetupWorld(Engine::Instance());
        }
        else if (gameStats.scores <= 0)
        {
            gameStats.resetTimer -= deltaTime;
        }
    }
}