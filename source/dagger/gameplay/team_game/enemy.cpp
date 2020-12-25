#include "enemy.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"


// ----------------------------------------------------------
// shortcuts
#include "gameplay/team_game/movement.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/detection.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/common/simple_collisions.h"
#include "core/graphics/text.h"
#include "core/input/inputs.h"

#include <glm/gtc/epsilon.hpp>
#include<iostream>

using namespace team_game;

String run = "among_them_animations:bat";
String idle_ = "among_them_animations:goblin_idle";

void EnemyControllerSystem::Run()
{
		Engine::Registry().view<EnemyFSM::StateComponent>().each(
			[&](EnemyFSM::StateComponent& state_)
		{   auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity); 
		    if (ctrl.stopenemies) {
			auto& body = Engine::Registry().get<MovableBody>(state_.entity);
			body.allowed = false;
			}
			else {
				auto& inputshape = Engine::Registry().get<InputEnemiesFile>(state_.entity);
				if (inputshape.currentshape == "goblin") { idle_ = "among_them_animations:goblin_idle"; run = "among_them_animations:goblin_run"; }
				if (inputshape.currentshape == "slime") { idle_ = "among_them_animations:slime_idle"; run = "among_them_animations:slime_run"; }
				if (inputshape.currentshape == "bat") { idle_ = "among_them_animations:bat";  run = "among_them_animations:bat"; }
				m_EnemyStateMachine.Run(state_);
			}
			

		});
	
}

DEFAULT_ENTER(EnemyFSM, Patrolling);

void EnemyFSM::Patrolling::Run(EnemyFSM::StateComponent& state_)
{
	auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);
	auto& path = Engine::Registry().get<InputEnemiesFile>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& det = Engine::Registry().get<Detection>(state_.entity);

	if (det.detected && Engine::Registry().has<CharacterController>(det.who)) {
		ctrl.lastState = EEnemyState::Patrolling;
		GoTo(EEnemyState::Chasing, state_);
	}


	FileInputStream inFile{ path.pathname };

	Vector2 point;
	while (inFile >> point.x >> point.y) {
		ctrl.path.emplace_back(point);
	}
	ctrl.numberOfPoints = ctrl.path.size();

	auto nextPosition = ctrl.postition + 1;

	auto coords = ctrl.path[nextPosition];

	auto destinationX = coords.x + ctrl.offset.x;
	auto destinationY = coords.y + ctrl.offset.y;

	if (transform.position.x < destinationX) {
		ctrl.direction.x = 1;
		sprite.scale.x = 1;
	}
	else if (transform.position.x > destinationX) {
		ctrl.direction.x = -1;
		sprite.scale.x = -1;

	}
	else ctrl.direction.x = 0;
	if (transform.position.y < destinationY) {
		ctrl.direction.y = 1;
	}
	else if (transform.position.y > destinationY) {
		ctrl.direction.y = -1;
	}
	else { ctrl.direction.y = 0; }

	AnimatorPlay(animator, run);
	if (glm::abs(destinationX - transform.position.x) < 0.1f && glm::abs(destinationY - transform.position.y) < 0.1f)
	{
		ctrl.postition = (ctrl.postition + 1) % ctrl.numberOfPoints;
	}
	Vector2 direction = { destinationX - transform.position.x, destinationY - transform.position.y };
	body.movement = glm::normalize(direction) * ctrl.speed * Engine::DeltaTime();
}

DEFAULT_EXIT(EnemyFSM, Patrolling);

DEFAULT_ENTER(EnemyFSM,Chasing);

void EnemyFSM::Chasing::Run(EnemyFSM::StateComponent& state_)
{

	auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& det = Engine::Registry().get<Detection>(state_.entity);
	auto& t = Engine::Registry().get<Transform>(state_.entity);
	auto& col = Engine::Registry().get<SimpleCollision>(state_.entity);

	auto& heroTransform = Engine::Registry().get<Transform>(det.who);
	auto& heroDetection = Engine::Registry().get<Detection>(det.who);
	auto& hero = Engine::Registry().get<CharacterController>(det.who);

	if (!det.detected) {
		EEnemyState currState = ctrl.lastState;
		ctrl.lastState = EEnemyState::Chasing;
		GoTo(currState, state_);
	}
	if(hero.canMove==false) {ctrl.stopenemies = true;}
	if (ctrl.shape != hero.shape)
	{
		AnimatorPlay(animator, run);
		Vector2 direction = { heroTransform.position.x - t.position.x, heroTransform.position.y - t.position.y };

		if (direction.x > 0.0f)
		{
			sprite.scale.x = 1;
		}
		else if (direction.x < 0.0f)
		{
			sprite.scale.x = -1;
		}

		body.movement = glm::normalize(direction) * ctrl.speed * Engine::DeltaTime();
		if (col.colided)
		{
			if (Engine::Registry().has<CharacterController>(col.colidedWith))
			{   
			    
				
				auto ui = Engine::Registry().create();
				auto& text = Engine::Registry().emplace<Text>(ui);
				text.spacing = 0.6f;
				text.Set("pixel-font", "You lose");
				auto ui2 = Engine::Registry().create();
				auto& text2 = Engine::Registry().emplace<Text>(ui2);
				text2.spacing = 0.4f;
				text2.position = { 0,-50,0 };
				text2.Set("pixel-font", "Press R to restart");
				hero.canMove = false;
				ctrl.stopenemies = true;
			}
			col.colided = false;
		}
		
	}
}

DEFAULT_EXIT(EnemyFSM, Chasing);


