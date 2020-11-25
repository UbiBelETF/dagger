#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include <glm/gtc/epsilon.hpp>

// ----------------------------------------------------------
// shortcuts

// you can call the states the same as in the enum
using Idle = CharacterControllerFSM::IdleState;
using Running = CharacterControllerFSM::RunningState;

// you can call this like the Internal structure you created
using CharacterController = CharacterControllerFSM::StateComponent;

constexpr Float32 CHAR_CONTROLLER_EPSILON = 0.000001f;

// ----------------------------------------------------------
// system stuff

void CharacterControllerSystem::Run()
{
	// we do all the inter-component work here to set up our controller so that it can be useful in the states.
	// remember that once in a state, that component is all you have, so link it in how ever you want.
	// for example, here we feed it some input data from the input component and point to the transform
	// to be able to change it (can't use references because they can't ever be null)
	Engine::Registry().view<CharacterController, InputReceiver, Transform>().each(
		[&](CharacterController& controller_, InputReceiver input_, Transform& transform_)
		{
			const Float32 horizontal = input_.Get("horizontalRun");
			const Float32 vertical = input_.Get("verticalRun");

			controller_.userInput = { horizontal, vertical };
			controller_.transform = &transform_;

			// and then we just run the machine!
			m_CharStateMachine.Run(controller_);
		});
}

// ----------------------------------------------------------
// fsm stuff
// idle

void Idle::Run(CharacterController& ctrl_)
{
	// it's unsafe to compare floats precisely so we use epsilon
	if (glm::epsilonNotEqual(glm::length(ctrl_.userInput), 0.0f, CHAR_CONTROLLER_EPSILON))
	{
		GoTo(ECharacterState::Running, ctrl_);
	}
}

// ----------------------------------------------------------
// running

void Running::Run(CharacterController& ctrl_)
{
	if (glm::epsilonEqual(glm::length(ctrl_.userInput), 0.0f, CHAR_CONTROLLER_EPSILON))
	{
		GoTo(ECharacterState::Idle, ctrl_);
		return;
	}

	// what was that math with Sqrt(2)? go review trig, guys! :D
	Vector2 xy = glm::normalize(ctrl_.userInput) * ctrl_.speed * Engine::DeltaTime();
	ctrl_.transform->position.x += xy.x;
	ctrl_.transform->position.y += xy.y;
}
