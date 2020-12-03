#include "CharacterController.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"

using namespace dagger;

void CharacterController::Idle::Enter(CharacterController::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "parti_team_game:Characters:Knight");
}
void CharacterController::Idle::Exit(CharacterController::StateComponent& state_)
{}

void CharacterController::Idle::Run(CharacterController::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("slide")))
	{
		GoTo(CharacterStates::Sliding, state_);
	}
}

void CharacterController::Sliding::Enter(CharacterController::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "parti_team_game:Characters:Knight");
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void CharacterController::Sliding::Exit(CharacterController::StateComponent& state_)
{}

void CharacterController::Sliding::Run(CharacterController::StateComponent& state_)
{
	/*auto& [sprite, input, character] = Engine::Registry()
		.get<Sprite, InputReceiver, platformer::PlatformerCharacter>(state_.entity);
		
	Float32 slide = input.Get("slide");

	if (EPSILON_ZERO(slide))
	{
		GoTo(CharacterStates::Idle, state_);
	}
	else
	{
		sprite.scale.x = run;
		sprite.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
	}*/
}