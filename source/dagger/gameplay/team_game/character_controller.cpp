#include "character_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace team_game;

void CharacterControllerSystem::Run()
{
	Engine::Registry().view<CharacterControllerFSM::StateComponent>()
		.each([&](CharacterControllerFSM::StateComponent& state_)
			{
				characterFSM.Run(state_);
			});
}


//OLD FSM

/*#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include <glm/gtc/epsilon.hpp>


using namespace team_game;

void CharacterControllerSystem::Run()
{
	Engine::Registry().view<CharacterController, InputReceiver, Transform>().each(
		[&](CharacterController& controller_, InputReceiver& input_, Transform& transform_)
		{
			controller_.userInput = &input_;
			controller_.transform = &transform_;

			m_CharStateMachine.Run(controller_);
		});
}

void CharacterControllerFSM::IdleState::Run(CharacterController& ctrl_)
{
	if (EPSILON_NOT_EQUAL(ctrl_.userInput->values.at("run"), 0.0f))
	{
		GoTo(ECharacterState::Running, ctrl_);
		return;
	}
	//play idle animation
}

void CharacterControllerFSM::RunningState::Run(CharacterController& ctrl_)
{
	if (EPSILON_EQUAL(ctrl_.userInput->values.at("run"), 0.0f))
	{
		GoTo(ECharacterState::Idle, ctrl_);
		return;
	}
	//play running animation
	ctrl_.transform->position.x += ctrl_.userInput->values.at("run") * ctrl_.speed * Engine::DeltaTime();
}*/