#include "racing_game_logic.h"
#include "racing_car.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/racing/racing_player_car.h"
#include "gameplay/racing/racing_main.h"

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

        auto view = Engine::Registry().view<RacingPlayerCar, Transform, SimpleCollision>();
        for (auto entity : view)
        {
            auto &t = view.get<Transform>(entity);
            auto &player = view.get<RacingPlayerCar>(entity);
            auto &col = view.get<SimpleCollision>(entity);

            if (col.colided)
            {
                Engine::Registry().view<RacingCar>().each([&](RacingCar& car_) {
                    car_.speed = 0;
                });
                Engine::Registry().view<RacingPlayerCar>().each([&](RacingPlayerCar& car_) {
                    car_.horzSpeed = 0;
                });
              //  m_Restart = true;
            }
        }
    }
}

void RacingCollisionsLogicSystem::OnEndOfFrame()
{/*
    if (m_Restart)
    {
        m_Restart = false;
        Engine::Registry().clear();

        racing_game::SetupWorld(Engine::Instance());
    }*/
}