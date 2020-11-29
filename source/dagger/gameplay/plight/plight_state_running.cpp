#include "plight_state_running.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/plight/plight_state_idle.h"
#include "gameplay/plight/plight_combat.h"

using namespace plight;

void plight::PlightRunningStateSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "moveX", "moveY", "use" })
    {
        receiver.values[command] = 0;
    }
}

void plight::PlightRunningStateSystem::Run()
{
    Engine::Registry().view<StateRunning, InputReceiver, Sprite, Transform, Animator,CombatStats>().each([&](StateRunning& state_, InputReceiver& input_,
        Sprite& sprite_, Transform& transform_, Animator& animator_, CombatStats& cstats_) {
        Float32 moveX = input_.values.at("moveX");
        Float32 moveY = input_.values.at("moveY");

        AnimatorPlay(animator_, "Plight:big_deamon:RUN"); // Play the animation for the current state
        if (state_.justEntered) { //Move the character the first time entering running state
            if (state_.enterX) sprite_.scale.x = state_.enterX;
            sprite_.position.x += state_.speed * state_.enterX * Engine::DeltaTime();
            transform_.position.x += state_.speed * state_.enterX * Engine::DeltaTime();

            sprite_.position.y += state_.speed * state_.enterY * Engine::DeltaTime();
            transform_.position.y += state_.speed * state_.enterY * Engine::DeltaTime();
            state_.justEntered = false;
        }
        

        if ((moveX == 0 && moveY == 0) || cstats_.currentStamina < STAMINA_FOR_RUNNING_FRAME)
        {
            state_.nextState = EPlightState::idle;
        }
        else {
            if (moveX) sprite_.scale.x = moveX;
            sprite_.position.x += state_.speed * moveX * Engine::DeltaTime();
            transform_.position.x += state_.speed * moveX * Engine::DeltaTime();

            sprite_.position.y += state_.speed * moveY * Engine::DeltaTime();
            transform_.position.y += state_.speed * moveY * Engine::DeltaTime();
        }

        

        });
}

void plight::PlightRunningStateSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&PlightRunningStateSystem::OnInitialize>(this);
    Engine::Dispatcher().sink<NextFrame>().connect<&PlightRunningStateSystem::OnEndOfFrame>(this);
}

void plight::PlightRunningStateSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlightRunningStateSystem::OnInitialize>(this);
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightRunningStateSystem::OnEndOfFrame>(this);
}

void plight::PlightRunningStateSystem::OnEndOfFrame()
{
    auto& reg = Engine::Registry();
    auto view = reg.view<StateRunning>();
    for (auto entity : view) {
        auto& state = reg.get<StateRunning>(entity);
        if (state.nextState != EPlightState::running) {
            reg.remove<StateRunning>(entity);
            reg.emplace<StateIdle>(entity);
        }
    }
}
