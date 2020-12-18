#include "enemy_controller_fsm.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/character.h"
#include "gameplay/team_game/gravity.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/team_game_collisions.h"

using namespace dagger;

// Idle

void team_game::EnemyControllerFSM::Idle::Enter(EnemyControllerFSM::StateComponent& state_)
{
	//	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	//	idle animation
}

DEFAULT_EXIT(team_game::EnemyControllerFSM, Idle);

void team_game::EnemyControllerFSM::Idle::Run(EnemyControllerFSM::StateComponent& state_)
{
	auto& actions = Engine::Registry().get<ActionManager>(state_.entity);

	if (EPSILON_NOT_ZERO(actions.run))
	{
		GoTo(EEnemyStates::Running, state_);
	}
	if (EPSILON_NOT_ZERO(actions.jump))
	{
		GoTo(EEnemyStates::Jumping, state_);
	}
}


// Running

void team_game::EnemyControllerFSM::Running::Enter(EnemyControllerFSM::StateComponent& state_)
{
	//printf("ENTERED RUNNING\n");
	//	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	//	run animation
}

// same as: DEFAULT_EXIT(CharacterControllerFSM, Running);
void team_game::EnemyControllerFSM::Running::Exit(EnemyControllerFSM::StateComponent& state_)
{
	//printf("EXIT RUNNING\n");
}

void team_game::EnemyControllerFSM::Running::Run(EnemyControllerFSM::StateComponent& state_)
{
	
	auto&& [sprite, transform, actions, character, enemyCharacter] = Engine::Registry().
													get<Sprite, Transform, ActionManager, Character, EnemyCharacter>(state_.entity);

	Float32 run = actions.run;
	Float32 jump = actions.jump;

	if (EPSILON_ZERO(jump))
	{
		if (EPSILON_ZERO(run))
		{
			GoTo(EEnemyStates::Idle, state_);
		}
		else
		{
			sprite.scale.x = run;
			transform.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
		}
	}
	else
	{
		GoTo(EEnemyStates::Jumping, state_);
	}
	
}

