#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include <glm/gtc/epsilon.hpp>

//useful strings
String idle = "among_them_animations:knight_idle";
String running = "among_them_animations:knight_run";

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
	Engine::Registry().view<CharacterController, InputReceiver, Transform, Sprite, Animator>().each(
		[&](CharacterController& controller_, InputReceiver input_, Transform& transform_, Sprite& sprite_,
			Animator& animator_)
		{
			const Float32 horizontal = input_.Get("horizontalRun");
			const Float32 vertical = input_.Get("verticalRun");

			const Float32 goblin = input_.Get("goblinTransform");
			const Float32 slime = input_.Get("slimeTransform");
			const Float32 bat = input_.Get("batTransform");
			const Float32 knight = input_.Get("knightTransform");

			controller_.userInput = { horizontal, vertical };
			controller_.shapeshiftInput = { goblin,slime,bat,knight };
			controller_.transform = &transform_;
			controller_.sprite = &sprite_;
			controller_.animator = &animator_;
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
	if (ctrl_.shapeshiftInput.x == 1) { idle = "among_them_animations:goblin_idle"; running = "among_them_animations:goblin_run"; 	}
	if (ctrl_.shapeshiftInput.y == 1) { idle = "among_them_animations:slime_idle"; running = "among_them_animations:slime_run";}
	if (ctrl_.shapeshiftInput.z == 1) { idle = "among_them_animations:bat"; running = "among_them_animations:bat"; }
	if (ctrl_.shapeshiftInput.w == 1) { idle = "among_them_animations:knight_idle"; running = "among_them_animations:knight_run"; }
	AnimatorPlay(*ctrl_.animator, idle);
}

// running

void Running::Run(CharacterController& ctrl_)
{
	if (EPSILON_EQUAL(glm::length(ctrl_.userInput), 0.0f))
	{
		GoTo(ECharacterState::Idle, ctrl_);
		return;
	}

	// Character orientation
	if (ctrl_.userInput.x > 0.0f)
	{
		ctrl_.sprite->scale.x = 1;
	}
	else if (ctrl_.userInput.x < 0.0f)
	{
		ctrl_.sprite->scale.x = -1;
	}
	if (ctrl_.shapeshiftInput.x == 1) { running = "among_them_animations:goblin_run"; idle = "among_them_animations:goblin_idle"; }
	if (ctrl_.shapeshiftInput.y == 1) { running = "among_them_animations:slime_run"; idle = "among_them_animations:slime_idle"; }
	if (ctrl_.shapeshiftInput.z == 1) { running = "among_them_animations:bat"; idle = "among_them_animations:bat"; }
	if (ctrl_.shapeshiftInput.w == 1) { running = "among_them_animations:knight_run"; idle = "among_them_animations:knight_idle"; }
	AnimatorPlay(*ctrl_.animator, running);
	Vector2 xy = glm::normalize(ctrl_.userInput) * ctrl_.speed * Engine::DeltaTime();
	ctrl_.transform->position.x += xy.x;
	ctrl_.transform->position.y += xy.y;
}
