#include "enemy.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"


// ----------------------------------------------------------
// shortcuts
#include "gameplay/team_game/movement.h"

#include <glm/gtc/epsilon.hpp>

using namespace team_game;

void EnemyControllerSystem::Run()
{
	Engine::Registry().view<EnemyFSM::StateComponent>().each(
		[&](EnemyFSM::StateComponent& state_)
	{
		
		m_EnemyStateMachine.Run(state_);
	});

}

DEFAULT_ENTER(EnemyFSM, Patrolling);

void EnemyFSM::Patrolling::Run(EnemyFSM::StateComponent& state_)
{
	auto& ctrl = Engine::Registry().get<EnemyController>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	
}

DEFAULT_EXIT(EnemyFSM, Patrolling);

DEFAULT_ENTER(EnemyFSM,Chasing);

void EnemyFSM::Chasing::Run(EnemyFSM::StateComponent& state_)
{
	auto& ctrl = Engine::Registry().get<EnemyController>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	

	

	
}

DEFAULT_EXIT(EnemyFSM, Chasing);
