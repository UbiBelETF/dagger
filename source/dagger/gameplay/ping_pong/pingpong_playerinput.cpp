#include "pingpong_playerinput.h"
#include "pingpong_ball.h"
#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;
bool PingPongPlayerInputSystem::s_PowerUpActive = false;
bool PingPongPlayerInputSystem::s_ActivatePowerUp = false;
bool PingPongPlayerInputSystem::s_DeactivatePowerUp = false;
TimePoint PingPongPlayerInputSystem::s_PowerUpStartTime = {};

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
    auto view = Engine::Registry().view<ControllerMapping, PlayerPowerUp>();

    for(auto entity : view)
    {
        auto& ctrl_ = view.get<ControllerMapping>(entity);
        auto& pwrups_ = view.get<PlayerPowerUp>(entity);

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
                if (pwrups_.slow_down > 0 && s_PowerUpActive == false && s_ActivatePowerUp == false) {
                    pwrups_.power_up = true;
                    s_ActivatePowerUp = true;
                }

                else if (s_PowerUpActive == true && pwrups_.power_up == true){
                    pwrups_.power_up = false;
                    pwrups_.power_down = true;
                    s_DeactivatePowerUp = true;
                }
            }
    };
}

void PingPongPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping, PlayerPowerUp>();
    for (auto entity : view)
    {
        auto& t = view.get<Transform>(entity);
        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& pwrup = view.get<PlayerPowerUp>(entity);

        //If power-up has been active for more than 5s , then it deactivates automaticaly
        if (s_PowerUpActive == true && std::chrono::duration_cast<std::chrono::milliseconds>(TimeSnapshot() - s_PowerUpStartTime).count() > 5000) {
            Engine::Registry().view<PlayerPowerUp>().each([&](PlayerPowerUp& pwrup){
                if (pwrup.power_up == true) {
                    pwrup.power_up = false;
                    pwrup.power_down = true;
                    s_DeactivatePowerUp = true;
                }
            });

            
        }

        if (pwrup.power_up == true && s_PowerUpActive == false && s_ActivatePowerUp == true) {
                pwrup.slow_down--;
            
                s_PlayerSpeed = s_PlayerSpeed * 0.5f;
                s_PowerUpActive = true;
                s_ActivatePowerUp = false;

                Engine::Registry().view<PingPongBall>().each([&](PingPongBall& ball) {

                    if (ball.reachedGoal)
                    {
                        return;
                    }

                    ball.speed.x = ball.speed.x * 0.33f;
                    ball.speed.y = ball.speed.y * 0.33f;

                });
                s_PowerUpStartTime = TimeSnapshot();
        }
        else if (pwrup.power_down == true && s_DeactivatePowerUp == true) {
            s_DeactivatePowerUp = true;
            s_PlayerSpeed = s_PlayerSpeed * 2;
            s_PowerUpActive = false;
            s_DeactivatePowerUp = false;

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
