#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include <glm/gtc/epsilon.hpp>

// ----------------------------------------------------------
// shortcuts

// you can call the states the same as in the enum
// it's a bother to write the whole thing out
using Idle = CharacterControllerFSM::IdleState;
using Running = CharacterControllerFSM::RunningState;

// ----------------------------------------------------------
// system stuff

void CharacterControllerSystem::Run()
{
	Engine::Registry().view<CharacterController, InputReceiver, Transform>().each(
		[&](CharacterController& controller_, InputReceiver input_, Transform& transform_)
		{
			const Float32 horizontal = input_.Get("horizontalRun");
			const Float32 vertical = input_.Get("verticalRun");

			controller_.userInput = { horizontal, vertical };
			controller_.transform = &transform_;

			m_CharStateMachine.Run(controller_);
		});
}

// ----------------------------------------------------------
// fsm stuff

// idle

void Idle::Run(CharacterController& ctrl_)
{
	if (EPSILON_NOT_EQUAL(glm::length(ctrl_.userInput), 0.0f))
	{
		GoTo(ECharacterState::Running, ctrl_);
	}
}

// running

void Running::Run(CharacterController& ctrl_)
{
	if (EPSILON_EQUAL(glm::length(ctrl_.userInput), 0.0f))
	{
		GoTo(ECharacterState::Idle, ctrl_);
		return;
	}

	Vector2 xy = glm::normalize(ctrl_.userInput) * ctrl_.speed * Engine::DeltaTime();
	ctrl_.transform->position.x += xy.x;
	ctrl_.transform->position.y += xy.y;
}
