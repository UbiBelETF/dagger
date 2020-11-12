#include "pingpong_playerinput.h"
#include "pingpong_ball.h"
#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;
bool PingPongPlayerInputSystem::s_PowerUpsActive = false;
bool PingPongPlayerInputSystem::s_ActivatePowerUps = false;
bool PingPongPlayerInputSystem::s_DeactivatePowerUps = false;

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
    auto view = Engine::Registry().view<ControllerMapping, PlayerPowerUps>();

    for(auto entity : view)
    {
        auto& ctrl_ = view.get<ControllerMapping>(entity);
        auto& pwrups_ = view.get<PlayerPowerUps>(entity);

            if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
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
            else if (kEvent_.key == ctrl_.slow_down_key && kEvent_.action == EDaggerInputState::Pressed) {
                if (pwrups_.slow_down > 0 && s_PowerUpsActive == false && s_ActivatePowerUps == false) {
                    pwrups_.power_up = true;
                    s_ActivatePowerUps = true;
                }

                else if (s_PowerUpsActive == true && pwrups_.power_up == true){
                    pwrups_.power_up = false;
                    pwrups_.power_down = true;
                    s_DeactivatePowerUps = true;
                }
            }
    };
}

void PingPongPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, PlayerPowerUps>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& pwrup = view.get<PlayerPowerUps>(entity);

        if (pwrup.power_up == true && s_PowerUpsActive == false && s_ActivatePowerUps == true) {
                pwrup.slow_down--;
            
                s_PlayerSpeed = s_PlayerSpeed * 0.5f;
                s_PowerUpsActive = true;
                s_ActivatePowerUps = false;

                Engine::Registry().view<PingPongBall>().each([&](PingPongBall& ball) {

                    if (ball.reachedGoal)
                    {
                        return;
                    }

                    ball.speed.x = ball.speed.x * 0.33f;
                    ball.speed.y = ball.speed.y * 0.33f;

                });
           
        }
        else if (pwrup.power_down == true && s_DeactivatePowerUps == true) {
            s_DeactivatePowerUps = true;
            s_PlayerSpeed = s_PlayerSpeed * 2;
            s_PowerUpsActive = false;
            s_DeactivatePowerUps = false;

            Engine::Registry().view<PingPongBall>().each([&](PingPongBall& ball) {

                if (ball.reachedGoal)
                {
                    return;
                }

                ball.speed.x = ball.speed.x / 0.33f;
                ball.speed.y = ball.speed.y / 0.33f;

            });
        }

        t.position.y += ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();

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
