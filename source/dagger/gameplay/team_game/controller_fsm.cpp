#include "controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/animations_fsm.h"
using namespace dagger;
using namespace team_game;

void team_game::ControllerFSM::Run(StateComponent& component_) 
{
    FSM<ECharacterStates>::Run(component_);
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(component_.entity);
    animationsFSM.Run(anim);
}
//idle
void ControllerFSM::Idle::Enter(ControllerFSM::StateComponent& state_) 
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Idle, anim);
}

void ControllerFSM::Idle::Run(ControllerFSM::StateComponent& state_) 
{
    auto&& [input,physics_] = Engine::Registry().get<InputReceiver,Physics>(state_.entity);
    if (EPSILON_NOT_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(ECharacterStates::Running, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y!=GetGravity()*Engine::DeltaTime() )
        GoTo(ECharacterStates::InAir, state_);
}

DEFAULT_EXIT(ControllerFSM, Idle);



//run
void ControllerFSM::Running::Enter(ControllerFSM::StateComponent& state_)
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Running, anim);
}
void ControllerFSM::Running::Run(ControllerFSM::StateComponent& state_) {
    auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics>(state_.entity);
    if (EPSILON_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(ECharacterStates::Idle, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y != GetGravity() * Engine::DeltaTime()) {
        GoTo(ECharacterStates::InAir,state_);
    }
    else
    {
        sprite_.scale.x = input.values.at("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
}

DEFAULT_EXIT(ControllerFSM, Running);

//in_air
void ControllerFSM::InAir::Enter(ControllerFSM::StateComponent& state_)
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    if(EPSILON_NOT_ZERO(input_.Get("jump")))
    {
        auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
            ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Jumping, anim);
    }
    else
    {
        auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
        ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Falling, anim);
    }
}
void ControllerFSM::InAir::Run(ControllerFSM::StateComponent& state_) 
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_,transform_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics,Transform>(state_.entity);
    if (EPSILON_NOT_ZERO(input_.Get("jump")) && physics_.velocity.y == GetGravity() * Engine::DeltaTime())
    {
        physics_.velocity.y += char_.speed.y ;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run"))) {
        sprite_.scale.x = input_.Get("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run")) && physics_.velocity.y == GetGravity() * Engine::DeltaTime()) GoTo(ECharacterStates::Running, state_);
    else if (physics_.velocity.y == GetGravity() * Engine::DeltaTime()) GoTo(ECharacterStates::Idle, state_);
    
}
DEFAULT_EXIT(ControllerFSM, InAir);