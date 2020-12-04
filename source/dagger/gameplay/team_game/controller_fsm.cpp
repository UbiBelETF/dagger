#include "controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/team_game/character_controller.h"

using namespace dagger;

// Idle

void team_game::CharacterControllerFSM::Idle::Enter(CharacterControllerFSM::StateComponent& state_)
{
//	auto& animator = Engine::Registry().get<Animator>(state_.entity);
//	idle animation
}

DEFAULT_EXIT(team_game::CharacterControllerFSM, Idle);

void team_game::CharacterControllerFSM::Idle::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("run")))
	{
		GoTo(ECharacterStates::Running, state_);
	}
}


// Running

void team_game::CharacterControllerFSM::Running::Enter(CharacterControllerFSM::StateComponent& state_)
{
//	auto& animator = Engine::Registry().get<Animator>(state_.entity);
//	run animation
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void team_game::CharacterControllerFSM::Running::Exit(CharacterControllerFSM::StateComponent& state_)
{}

void team_game::CharacterControllerFSM::Running::Run(CharacterControllerFSM::StateComponent& state_)
{
	auto&& [sprite, input, character] = Engine::Registry().get<Sprite, InputReceiver, team_game::PlayerCharacter>(state_.entity);

	Float32 run = input.Get("run");

	if (EPSILON_ZERO(run))
	{
		GoTo(ECharacterStates::Idle, state_);
	}
	else
	{
		sprite.scale.x = run;
		sprite.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}
}
