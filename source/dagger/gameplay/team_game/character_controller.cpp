#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"

#include "gameplay/team_game/movement.h"

#include <glm/gtc/epsilon.hpp>

using namespace team_game;

void CharacterControllerSystem::Run()
{
	Engine::Registry().view<CharacterFSM::StateComponent>().each(
		[&](CharacterFSM::StateComponent& state_)
		{
			m_CharStateMachine.Run(state_);
		});
}

void CharacterFSM::Idle::Enter(CharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "among_them_animations:knight_idle");
}

void CharacterFSM::Idle::Run(CharacterFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	
	if (EPSILON_NOT_ZERO(input.Get("horizontalRun")) || EPSILON_NOT_ZERO(input.Get("verticalRun")))
	{
		GoTo(ECharacterState::Running, state_);
	}
}

DEFAULT_EXIT(CharacterFSM, Idle);

void CharacterFSM::Running::Enter(CharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "among_them_animations:knight_run");
}

void CharacterFSM::Running::Run(CharacterFSM::StateComponent& state_)
{
	auto& ctrl = Engine::Registry().get<CharacterController>(state_.entity);
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	float runX = input.Get("horizontalRun");
	float runY = input.Get("verticalRun");

	if (EPSILON_ZERO(runX) && EPSILON_ZERO(runY))
	{
		GoTo(ECharacterState::Idle, state_);
		return;
	}

	// Character orientation
	if (runX > 0.0f)
	{
		sprite.scale.x = 3;
	}
	else if (runX < 0.0f)
	{
		sprite.scale.x = -3;
	}

	body.movement = glm::normalize(Vector2{ runX, runY }) * ctrl.speed * Engine::DeltaTime();
}

DEFAULT_EXIT(CharacterFSM, Running);
