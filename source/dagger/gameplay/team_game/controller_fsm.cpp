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

//idle
DEFAULT_ENTER(ControllerFSM, Idle);

void ControllerFSM::Idle::Run(ControllerFSM::StateComponent& state_) 
{
    auto&& [input,physics_] = Engine::Registry().get<InputReceiver,Physics>(state_.entity);
    if (EPSILON_NOT_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(CharacterStates::Running, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y!=gravity_acceleration*Engine::DeltaTime() )
        GoTo(CharacterStates::InAir, state_);
}

DEFAULT_EXIT(ControllerFSM, Idle);



//run
DEFAULT_ENTER(ControllerFSM, Running);
void ControllerFSM::Running::Run(ControllerFSM::StateComponent& state_) {
    auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics>(state_.entity);
    if (EPSILON_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(CharacterStates::Idle, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y != gravity_acceleration * Engine::DeltaTime()) {
        GoTo(CharacterStates::InAir,state_);
    }
    else
    {
        sprite_.scale.x = input.values.at("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
}

DEFAULT_EXIT(ControllerFSM, Running);

//in_air
DEFAULT_ENTER(ControllerFSM, InAir);
void ControllerFSM::InAir::Run(ControllerFSM::StateComponent& state_) 
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_,transform_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics,Transform>(state_.entity);
    if (EPSILON_NOT_ZERO(input_.Get("jump")) && physics_.velocity.y == gravity_acceleration * Engine::DeltaTime())
    {
        physics_.velocity.y += char_.speed.y ;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run"))) {
        sprite_.scale.x = input_.Get("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run")) && physics_.velocity.y == gravity_acceleration * Engine::DeltaTime()) GoTo(CharacterStates::Running, state_);
    else if (physics_.velocity.y == gravity_acceleration * Engine::DeltaTime()) GoTo(CharacterStates::Idle, state_);
    
   
}
DEFAULT_EXIT(ControllerFSM, InAir);