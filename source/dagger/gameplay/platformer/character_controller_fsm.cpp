#include "gameplay/platformer/character_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/platformer/platformer_controller.h"

using namespace dagger;
using namespace platformer;

// Idle

void PlatformerCharacterControllerFSM::Idle::Enter(PlatformerCharacterControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "souls_like_knight_character:IDLE");
}

DEFAULT_EXIT(PlatformerCharacterControllerFSM, Idle);

void PlatformerCharacterControllerFSM::Idle::Run(PlatformerCharacterControllerFSM::StateComponent& state_) 
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("run")))
	{
		GoTo(EPlatformerCharacterStates::Running, state_);
	}
}

// Running

void PlatformerCharacterControllerFSM::Running::Enter(PlatformerCharacterControllerFSM::StateComponent& state_) 
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "souls_like_knight_character:RUN");
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void PlatformerCharacterControllerFSM::Running::Exit(PlatformerCharacterControllerFSM::StateComponent& state_) 
{}

void PlatformerCharacterControllerFSM::Running::Run(PlatformerCharacterControllerFSM::StateComponent& state_) 
{
	auto&& [sprite, input, character] = Engine::Registry().get<Sprite, InputReceiver, platformer::PlatformerCharacter>(state_.entity);

	Float32 run = input.Get("run");

	if (EPSILON_ZERO(run))
	{
		GoTo(EPlatformerCharacterStates::Idle, state_);
	}
	else
	{
		sprite.scale.x = run;
		sprite.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}
}
