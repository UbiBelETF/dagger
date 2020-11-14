#include "pingpong_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/ping_pong/pingpong_ball.h"

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;
bool PingPongPlayerInputSystem::s_PowerUpActive = false;
bool PingPongPlayerInputSystem::s_ActivatePowerUp = false;
bool PingPongPlayerInputSystem::s_DeactivatePowerUp = false;
Float32 PingPongPlayerInputSystem::s_PowerUpDuration = { 5.0f };
Float32 PingPongPlayerInputSystem::s_TimeUnitlNextChange = { 5.0f };

const Float32 playerSlowMultiplier = 0.5f;
const Float32 ballSlowMultiplier = 0.33f;

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

        auto& ctrl = view.get<ControllerMapping>(entity);
        auto& pwrups = view.get<PlayerPowerUp>(entity);

             if (kEvent_.key == ctrl.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl.input.y = 1*ctrl.inverted;
        }
        else if (kEvent_.key == ctrl.up_key && kEvent_.action == EDaggerInputState::Released && ((ctrl.input.y > 0 && ctrl.inverted == 1) || (ctrl.input.y < 0 && ctrl.inverted == -1)))
        {
            ctrl.input.y = 0;
        }
        else if (kEvent_.key == ctrl.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl.input.y = -1*ctrl.inverted;
        }
        else if (kEvent_.key == ctrl.down_key && kEvent_.action == EDaggerInputState::Released && ((ctrl.input.y < 0 && ctrl.inverted == 1) || (ctrl.input.y > 0 && ctrl.inverted == -1)))
        {
            ctrl.input.y = 0;
        }
            else if (kEvent_.key == ctrl.slow_down_key && kEvent_.action == EDaggerInputState::Pressed) {
                if (pwrups.slow_down > 0 && s_PowerUpActive == false && s_ActivatePowerUp == false) {
                    pwrups.power_up = true;
                    s_ActivatePowerUp = true;
                }

                else if (s_PowerUpActive == true && pwrups.power_up == true){
                    pwrups.power_up = false;
                    pwrups.power_down = true;
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
        if (s_PowerUpActive == true) {
            s_TimeUnitlNextChange -= dagger::Engine::Instance().DeltaTime();
            if(s_TimeUnitlNextChange <= 0){
                Engine::Registry().view<PlayerPowerUp>().each([&](PlayerPowerUp& pwrup) {
                    if (pwrup.power_up == true) {
                        pwrup.power_up = false;
                        pwrup.power_down = true;
                        s_DeactivatePowerUp = true;
                    }
                
                });
                s_TimeUnitlNextChange = s_PowerUpDuration;
            }
            
        }

        if (pwrup.power_up == true && s_PowerUpActive == false && s_ActivatePowerUp == true) {
                pwrup.slow_down--;
            
                s_PlayerSpeed = s_PlayerSpeed * playerSlowMultiplier;
                s_PowerUpActive = true;
                s_ActivatePowerUp = false;

                Engine::Registry().view<PingPongBall>().each([&](PingPongBall& ball) {

                    if (ball.reachedGoal)
                    {
                        return;
                    }

                    ball.speedMultiplier = ballSlowMultiplier;

                });
        }
        else if (pwrup.power_down == true && s_DeactivatePowerUp == true) {
            s_DeactivatePowerUp = true;
            s_PlayerSpeed = s_PlayerSpeed / playerSlowMultiplier;
            s_PowerUpActive = false;
            s_DeactivatePowerUp = false;

            Engine::Registry().view<PingPongBall>().each([&](PingPongBall& ball) {

                if (ball.reachedGoal)
                {
                    return;
                }

                ball.speedMultiplier = 1.f;

            });
        }

        if (ctrl.inverted == -1) {
            ctrl.timeUnitlNextChange -= dagger::Engine::Instance().DeltaTime();
            if (ctrl.timeUnitlNextChange <= 0) {
                ctrl.inverted = 1;
                ctrl.timeUnitlNextChange = ctrl.invertedTimePeriod;
            }
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
