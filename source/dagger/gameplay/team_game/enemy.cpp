#include "enemy.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animations.h"


// ----------------------------------------------------------
// shortcuts
#include "gameplay/team_game/movement.h"

#include <glm/gtc/epsilon.hpp>
#include <core\game\transforms.h>

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
	auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& transform = Engine::Registry().get<Transform>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	auto& animator = Engine::Registry().get<Animator>(state_.entity);

	FileInputStream inFile{ "path.txt" };

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

	AnimatorPlay(animator, "among_them_animations:goblin_run");
	transform.position.x += ctrl.direction.x * ctrl.speed * Engine::DeltaTime();
	transform.position.y += ctrl.direction.y * ctrl.speed * Engine::DeltaTime();

	if ((transform.position.x < destinationX && ctrl.direction.x == -1) ||
		(transform.position.x > destinationX && ctrl.direction.x == 1)) {
		transform.position.x = destinationX;
	}

	else if ((transform.position.y > destinationY && ctrl.direction.y == 1) ||
		(transform.position.y < destinationY && ctrl.direction.y == -1))
	{
		transform.position.y = destinationY;
	}

	if (transform.position.x == destinationX && transform.position.y == destinationY) {
		ctrl.postition++;
		if ((ctrl.postition + 1) >= ctrl.numberOfPoints) {
			return;
		}
		
	}

	
}

DEFAULT_EXIT(EnemyFSM, Patrolling);

DEFAULT_ENTER(EnemyFSM,Chasing);

void EnemyFSM::Chasing::Run(EnemyFSM::StateComponent& state_)
{
	/*auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	*/

	

	
}

DEFAULT_EXIT(EnemyFSM, Chasing);
