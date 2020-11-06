#include "racing_player_car.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/racing/racing_game_logic.h"

using namespace dagger;
using namespace racing_game;

void RacingPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&RacingPlayerInputSystem::OnKeyboardEvent>(this);
}

void RacingPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&RacingPlayerInputSystem::OnKeyboardEvent>(this);
}

void RacingPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x < 0)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released && ctrl_.input.x > 0)
        {
            ctrl_.input.x = 0;
        }
    });
}

void RacingPlayerInputSystem::Run()
{
    RacingGameFieldSettings fieldSettings;
    if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
    {
        fieldSettings = *ptr;
    }

    auto view = Engine::Registry().view<Transform, ControllerMapping, RacingPlayerCar>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);
        auto &car = view.get<RacingPlayerCar>(entity);

        t.position.x += ctrl.input.x * car.horzSpeed * Engine::DeltaTime();

        Float32 boarderX = fieldSettings.GetXBoarder();
        if (t.position.x > boarderX)
        {
            t.position.x = boarderX;
        }

        if (t.position.x < -boarderX)
        {
            t.position.x = -boarderX;
        }
    }
}

