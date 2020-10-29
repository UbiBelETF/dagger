#include "pingpong_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace pingPong;

Float32 PingPongPlayerInputSystem::BOARDER_DOWN = -20;
Float32 PingPongPlayerInputSystem::BOARDER_UP = 20;

Float32 PingPongPlayerInputSystem::PLAYER_SPEED = 1.f;

void PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent.key == ctrl_.up_key && (kEvent.action == DaggerInputState::Pressed || kEvent.action == DaggerInputState::Held))
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent.key == ctrl_.up_key && kEvent.action == DaggerInputState::Released && ctrl_.input.y > 0)
        {
            ctrl_.input.y = 0;
        }
        else if (kEvent.key == ctrl_.down_key && (kEvent.action == DaggerInputState::Held || kEvent.action == DaggerInputState::Pressed))
        {
            ctrl_.input.y = -1;
        }
        else if (kEvent.key == ctrl_.down_key && kEvent.action == DaggerInputState::Released && ctrl_.input.y < 0)
        {
            ctrl_.input.y = 0;
        }
    });
}

void PingPongPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);

        t.position.y += ctrl.input.y * PLAYER_SPEED * Engine::DeltaTime();

        if (t.position.y > BOARDER_UP)
        {
            t.position.y = BOARDER_UP;
        }

        if (t.position.y < BOARDER_DOWN)
        {
            t.position.y = BOARDER_DOWN;
        }
    }
}
