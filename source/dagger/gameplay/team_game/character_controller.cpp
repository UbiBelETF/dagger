#include "character_controller.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"

#include <glm/gtc/epsilon.hpp>

//useful strings
String idle = "among_them_animations:knight_idle";
String running = "among_them_animations:knight_run";

// ----------------------------------------------------------
// shortcuts
#include "gameplay/team_game/movement.h"

#include <glm/gtc/epsilon.hpp>

using namespace team_game;

void CharacterControllerSystem::Run()
{
	auto view = Engine::Registry().view<CharacterController>();
	for (auto entity : view) {
		auto& chController = view.get<CharacterController>(entity);
		Engine::Registry().view<CharacterFSM::StateComponent>().each(
			[&](CharacterFSM::StateComponent& state_)
		{   if (chController.canMove) {
			auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
			if (input.Get("goblinTransform") == 1) { idle = "among_them_animations:goblin_idle"; running = "among_them_animations:goblin_run"; chController.SetShape(ECharacterShape::Goblin); }
			if (input.Get("slimeTransform") == 1) { idle = "among_them_animations:slime_idle"; running = "among_them_animations:slime_run"; chController.SetShape(ECharacterShape::Slime); }
			if (input.Get("batTransform") == 1) { idle = "among_them_animations:bat"; running = "among_them_animations:bat"; chController.SetShape(ECharacterShape::Bat); }
			if (input.Get("knightTransform") == 1) { idle = "among_them_animations:knight_idle"; running = "among_them_animations:knight_run"; chController.SetShape(ECharacterShape::Hero); }
			m_CharStateMachine.Run(state_);
		}
			});
	}
}

DEFAULT_ENTER(CharacterFSM, Idle);

void CharacterFSM::Idle::Run(CharacterFSM::StateComponent& state_)
{  
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	
	if (EPSILON_NOT_ZERO(input.Get("horizontalRun")) || EPSILON_NOT_ZERO(input.Get("verticalRun")))
	{
		GoTo(ECharacterState::Running, state_);
	}
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, idle);
}

DEFAULT_EXIT(CharacterFSM, Idle);

DEFAULT_ENTER(CharacterFSM, Running);

void CharacterFSM::Running::Run(CharacterFSM::StateComponent& state_)
{
	auto& ctrl = Engine::Registry().get<CharacterController>(state_.entity);
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, running);

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
		sprite.scale.x = 1;
	}
	else if (runX < 0.0f)
	{
		sprite.scale.x = -1;
	}

	body.movement = glm::normalize(Vector2{ runX, runY }) * ctrl.speed * Engine::DeltaTime();
}

DEFAULT_EXIT(CharacterFSM, Running);
