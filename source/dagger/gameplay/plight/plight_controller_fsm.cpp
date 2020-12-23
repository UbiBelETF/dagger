#define _USE_MATH_DEFINES

#include "plight_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include <cmath>
#include <math.h>

#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_aiming.h"

using namespace dagger;
using namespace plight;

// Idle


void PlightCharacterControllerFSM::Idle::Enter(PlightCharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Plight:knight_m:IDLE");
    auto& character = Engine::Registry().get<PlightCharacterController>(state_.entity);
    character.running = false; character.dashing = false;
    
}

DEFAULT_EXIT(PlightCharacterControllerFSM, Idle);

void PlightCharacterControllerFSM::Idle::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto&& [input, character, cstats] = Engine::Registry().get<InputReceiver, PlightCharacterController, CombatStats>(state_.entity);

    if (character.dead) {
        return;
    }

    if (character.doubleTap) {
        character.currentDoubleTapDuration += Engine::DeltaTime();
        if (character.currentDoubleTapDuration >= character.doubleTapDurationWindow) {
            character.doubleTap = false;
            character.currentDoubleTapDuration = 0.f;
        }
    }

    if (character.hit) {
        GoTo(PlightCharacterStates::Hit, state_);
    }

    Float32 move = input.Get("move");
        
        if (cstats.currentStamina < STAMINA_FOR_RUNNING_FRAME) {
            if (cstats.currentStamina < 0.f) {
                cstats.currentStamina = 0.f;
            }
            character.resting = true;
        }

        if (character.resting) {  //Check to see if the character recently drained all stamina and has to rest
            character.currentRestingTime += Engine::DeltaTime();
            if (character.currentRestingTime >= character.restingTime) {
                character.resting = false;
                character.currentRestingTime = 0.f;
            }
        }
        else if (EPSILON_NOT_ZERO(move)) {
            if (move > 0) {
                if (!character.doubleTap) {
                    character.doubleTap = true;
                }
                else {
                    character.dashing = true;
                    character.currentDoubleTapDuration = 0.f;
                    character.doubleTap = false;
                    GoTo(PlightCharacterStates::Dashing, state_);
                }
            }
            if (!character.dashing) {
                character.firstMoveInput = move;
                GoTo(PlightCharacterStates::Running, state_);
            }
            
        }

      
}

// Running

void PlightCharacterControllerFSM::Running::Enter(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto& animator = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator, "Plight:knight_m:RUN");

    auto&& [sprite, character, crosshair, transform] = Engine::Registry().get<Sprite, PlightCharacterController, PlightCrosshair, Transform>(state_.entity);

    character.running = true;

    Float32 dx = character.speed * cos(crosshair.angle) * Engine::DeltaTime() * character.firstMoveInput;
    Float32 dy = character.speed * sin(crosshair.angle) * Engine::DeltaTime() * character.firstMoveInput;

    transform.position.x += dx;
    transform.position.y += dy;
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void PlightCharacterControllerFSM::Running::Exit(PlightCharacterControllerFSM::StateComponent& state_)
{}

void PlightCharacterControllerFSM::Running::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
   
    auto&& [sprite, input, character,cstats,crosshair,transform] = Engine::Registry().get<Sprite, InputReceiver, PlightCharacterController,CombatStats,PlightCrosshair,Transform>(state_.entity);

    if (character.dead) {
        GoTo(PlightCharacterStates::Idle, state_);
        return;
    }
    if (character.doubleTap) {
        character.currentDoubleTapDuration += Engine::DeltaTime();
        if (character.currentDoubleTapDuration >= character.doubleTapDurationWindow) {
            character.doubleTap = false;
            character.currentDoubleTapDuration = 0.f;
        }
    }

    if (character.hit) {
        GoTo(PlightCharacterStates::Hit, state_);
    }

    Float32 move = input.Get("move");

    if (EPSILON_ZERO(move) || cstats.currentStamina < STAMINA_FOR_RUNNING_FRAME)
    {
        GoTo(PlightCharacterStates::Idle, state_);
    }
    else {
        Float32 dx = character.speed * cos(crosshair.angle) * Engine::DeltaTime() * move;
        Float32 dy = character.speed * sin(crosshair.angle) * Engine::DeltaTime() * move;

        transform.position.x += dx;
        transform.position.y += dy;
    }
}

//Dashing
void PlightCharacterControllerFSM::Dashing::Enter(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto& animator = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator, "Plight:knight_m:DASH");
    auto& character = Engine::Registry().get<PlightCharacterController>(state_.entity);
    character.resting = false; 
    character.running = false;


}

void PlightCharacterControllerFSM::Dashing::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto&& [sprite, character, cstats, crosshair, transform] = Engine::Registry().get<Sprite,PlightCharacterController, CombatStats, PlightCrosshair, Transform>(state_.entity);
   
    if (character.dead) {
        GoTo(PlightCharacterStates::Idle, state_);
        return;
    }

    if (character.hit) {
        GoTo(PlightCharacterStates::Hit, state_);
    }

    if (!character.dashing || cstats.currentStamina < STAMINA_FOR_DASHING_FRAME)
    {
        GoTo(PlightCharacterStates::Idle, state_);
    }
    else {
        Float32 dx = character.dashingSpeed * cos(crosshair.angle) * Engine::DeltaTime();
        Float32 dy = character.dashingSpeed * sin(crosshair.angle) * Engine::DeltaTime();

        transform.position.x += dx;
        transform.position.y += dy;
    }

    character.currentDashingTime += Engine::DeltaTime();
    if (character.currentDashingTime >= character.dashingTime) {
        character.dashing = false;
        character.currentDashingTime = 0.f;
        GoTo(PlightCharacterStates::Idle, state_);
    }
}

DEFAULT_EXIT(PlightCharacterControllerFSM, Dashing);

//Hit


void PlightCharacterControllerFSM::Hit::Enter(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto& animator = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator, "Plight:knight_m:HIT");
    auto& character = Engine::Registry().get<PlightCharacterController>(state_.entity);
    character.running = false; character.dashing = false; character.doubleTap = false;
}

DEFAULT_EXIT(PlightCharacterControllerFSM, Hit);

void PlightCharacterControllerFSM::Hit::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto& character = Engine::Registry().get<PlightCharacterController>(state_.entity);

    character.currentHitTime += Engine::DeltaTime();
    if (character.currentHitTime >= character.hitTime) {
        character.currentHitTime = 0.f;
        character.hit = false;
        GoTo(PlightCharacterStates::Idle, state_);
    }

}