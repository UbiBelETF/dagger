#include "plight_state_idle.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/plight/plight_state_running.h"
#include "gameplay/plight/plight_combat.h"

using namespace plight;

void plight::PlightIdleStateSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "moveX", "moveY", "use" })
    {
        receiver.values[command] = 0;
    }
}

void plight::PlightIdleStateSystem::Run()
{
	Engine::Registry().view<StateIdle, InputReceiver, Animator,CombatStats>().each([&](StateIdle& state_,InputReceiver& input_,Animator& animator_, CombatStats& cstats_) {
        Float32 moveX = input_.values.at("moveX");
        Float32 moveY = input_.values.at("moveY");

        AnimatorPlay(animator_, "Plight:big_deamon:IDLE"); // Play the animation for the current state

        if (cstats_.currentStamina < STAMINA_FOR_RUNNING_FRAME) {
            if (cstats_.currentStamina < 0.f) {
                cstats_.currentStamina = 0.f;
            }
            state_.resting = true;
        }

        if (state_.resting) {  //Check to see if the character recently drained all stamina and has to rest
            state_.currentRestingTime += Engine::DeltaTime();
            if (state_.currentRestingTime >= state_.restingTime) {
                state_.resting = false;
                state_.currentRestingTime = 0.f;
            }
        }
        else if (moveX || moveY) {
            state_.nextState = EPlightState::running;
            state_.nextRunX = moveX;
            state_.nextRunY = moveY;
        }

    });
}

void plight::PlightIdleStateSystem::SpinUp()
{
	Engine::Registry().on_construct<InputReceiver>().connect<&PlightIdleStateSystem::OnInitialize>(this);
    Engine::Dispatcher().sink<NextFrame>().connect<&PlightIdleStateSystem::OnEndOfFrame>(this);
}

void plight::PlightIdleStateSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&PlightIdleStateSystem::OnInitialize>(this);
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightIdleStateSystem::OnEndOfFrame>(this);
}

void plight::PlightIdleStateSystem::OnEndOfFrame()
{
    auto& reg = Engine::Registry();
    auto view = reg.view<StateIdle>();
    for (auto entity : view) {
        auto& state = reg.get<StateIdle>(entity);
        if (state.nextState != EPlightState::idle) {
            auto& run = reg.emplace<StateRunning>(entity);
            run.enterX = state.nextRunX;
            run.enterY = state.nextRunY;
            reg.remove<StateIdle>(entity);   
        }
    }
        
   
}
