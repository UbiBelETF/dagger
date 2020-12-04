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
	AnimatorPlay(animator, "Plight:big_deamon:IDLE");
    auto& character = Engine::Registry().get<PlightCharacterController>(state_.entity);
    character.running = false;
    
}

DEFAULT_EXIT(PlightCharacterControllerFSM, Idle);

void PlightCharacterControllerFSM::Idle::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto&& [input, character, cstats] = Engine::Registry().get<InputReceiver, PlightCharacterController, CombatStats>(state_.entity);


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
            character.firstMoveInput = move;
            GoTo(PlightCharacterStates::Running, state_);
        }

      
}

// Running

void PlightCharacterControllerFSM::Running::Enter(PlightCharacterControllerFSM::StateComponent& state_)
{
    auto& animator = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator, "Plight:big_deamon:RUN");

    auto&& [sprite, character, crosshair, transform] = Engine::Registry().get<Sprite, PlightCharacterController, PlightCrosshair, Transform>(state_.entity);

    character.running = true;

    Float32 dx = character.speed * cos(crosshair.angle) * Engine::DeltaTime() * character.firstMoveInput;
    Float32 dy = character.speed * sin(crosshair.angle) * Engine::DeltaTime() * character.firstMoveInput;

    transform.position.x += dx;
    transform.position.y += dy;

    auto& csprite = Engine::Registry().get<Sprite>(crosshair.crosshairSprite);

    Float32 x = crosshair.playerDistance * cos(crosshair.angle);
    Float32 y = crosshair.playerDistance * sin(crosshair.angle);


    csprite.position.x = x + sprite.position.x;
    csprite.position.y = y + sprite.position.y;

    if (crosshair.angle > 0.5 * M_PI && crosshair.angle < 1.5 * M_PI) {
        sprite.scale = { -1,1 };
    }
    else {
        sprite.scale = { 1,1 };
    }
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void PlightCharacterControllerFSM::Running::Exit(PlightCharacterControllerFSM::StateComponent& state_)
{}

void PlightCharacterControllerFSM::Running::Run(PlightCharacterControllerFSM::StateComponent& state_)
{
   
    auto&& [sprite, input, character,cstats,crosshair,transform] = Engine::Registry().get<Sprite, InputReceiver, PlightCharacterController,CombatStats,PlightCrosshair,Transform>(state_.entity);

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

        auto& csprite = Engine::Registry().get<Sprite>(crosshair.crosshairSprite);

        Float32 x = crosshair.playerDistance * cos(crosshair.angle);
        Float32 y = crosshair.playerDistance * sin(crosshair.angle);


        csprite.position.x = x + sprite.position.x;
        csprite.position.y = y + sprite.position.y;

        if (crosshair.angle > 0.5 * M_PI && crosshair.angle < 1.5 * M_PI) {
            sprite.scale = { -1,1 };
        }
        else {
            sprite.scale = { 1,1 };
        }
    }
}
