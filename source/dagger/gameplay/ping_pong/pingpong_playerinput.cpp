#include "pingpong_playerinput.h"
#include "pingpong_ball.h"//Dodato
#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;

void PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.key == ctrl_.esc_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.exit = true;
        }//ADDED
        else if (kEvent_.key == ctrl_.space_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.boost = true;
        }//ADDED
        else if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
        {
            ctrl_.input.y = 0;
        }
        else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.y = -1;
        }
        else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
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

        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
        
        
        if (ctrl.exit) exit(0);
        
        
        if (ctrl.boost == true) {
            auto ball = Engine::Registry().view<PingPongBall>();
            for (auto entity : ball) {
                auto& b = ball.get<PingPongBall>(entity);
                if (!b.boosted) {
                    b.speed.x *= 2;
                    b.speed.y *= 2;
                    b.boosted = true;
                }
                else {
                    b.speed.x *= 0.5;
                    b.speed.y *= 0.5;
                    b.boosted = false;
                }
                ctrl.boost = false;
                //ctrl.boost = true;
            }
        }//exit(0);//DODATO
        if (t.position.y > s_BoarderUp)
        {
            t.position.y = s_BoarderUp;
        }

        if (t.position.y < s_BoarderDown)
        {
            t.position.y = s_BoarderDown;
        }
    }
}
