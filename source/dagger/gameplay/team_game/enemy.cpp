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
#include <gameplay\team_game\detection.h>
#include <gameplay\team_game\character_controller.h>

using namespace team_game;

String run = "among_them_animations:bat";
String idle_ = "among_them_animations:goblin_idle";

void EnemyControllerSystem::Run()
{
	Engine::Registry().view<EnemyFSM::StateComponent>().each(
		[&](EnemyFSM::StateComponent& state_)
	{   
		auto& inputshape = Engine::Registry().get<InputEnemiesFile>(state_.entity);
		if (inputshape.currentshape == "goblin") { idle_ = "among_them_animations:goblin_idle"; run = "among_them_animations:goblin_run"; }
		if (inputshape.currentshape == "slime") { idle_ = "among_them_animations:slime_idle"; run = "among_them_animations:slime_run"; }
		if (inputshape.currentshape == "bat") { idle_ = "among_them_animations:bat";  run = "among_them_animations:bat"; }
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
	
	auto& ctrl = Engine::Registry().get<EnemyDescription>(state_.entity);
	auto& sprite = Engine::Registry().get<Sprite>(state_.entity);
	auto& body = Engine::Registry().get<MovableBody>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& det = Engine::Registry().get<Detection>(state_.entity);
	auto& t = Engine::Registry().get<Transform>(state_.entity);

	auto& heroTransform = Engine::Registry().get<Transform>(det.who);
	auto& heroDetection = Engine::Registry().get<Detection>(det.who);
	auto& hero = Engine::Registry().get<CharacterController>(det.who);

	if (ctrl.shape != hero.shape) {

		Vector2 detectionSides = det.GetDetectionSides(t.position, heroDetection, heroTransform.position);
		Vector3 lastSeen;
		Float32 dt = Engine::DeltaTime();


		int counter = 0;

		lastSeen = det.where;
	
		do
		{
			/*if (detectionSides.x > 0)
			{
				t.position.x -= ((ctrl.speed - 2) * dt);
			}
			if (detectionSides.x < 0)
			{
				t.position.x += ((ctrl.speed - 2) * dt);
			}
			if (detectionSides.y > 0);
			{
				t.position.y -= ((ctrl.speed - 2) * dt);
			}
			if (detectionSides.y < 0)
			{
				t.position.y += ((ctrl.speed - 2) * dt);
			}*/
			lastSeen = det.where;
			if (det.where.x > t.position.x) t.position.x += (ctrl.speed * dt);
			if (det.where.x < t.position.x) t.position.x -= (ctrl.speed * dt);
			if (det.where.y > t.position.y) t.position.y += (ctrl.speed * dt);
			if (det.where.y < t.position.y) t.position.y -= (ctrl.speed * dt);
			//counter++;
			//Logger::critical("infinite loop");
		} while (!(glm::distance(glm::abs(det.where), glm::abs(t.position)) >= 0 && glm::distance(glm::abs(det.where), glm::abs(t.position)) < 0.5));
		//while (det.IsDetected(heroTransform.position, det, t.position));

		//if (hero.shape == ctrl.shape) GoTo(EEnemyState::Patrolling, state_);


		/*do
		{
			if (lastSeen.x > t.position.x) t.position.x += ((ctrl.speed - 2) * dt);
			if (lastSeen.x < t.position.x) t.position.x -= ((ctrl.speed - 2) * dt);
			if (lastSeen.y > t.position.y) t.position.y += ((ctrl.speed - 2) * dt);
			if (lastSeen.y < t.position.y) t.position.y -= ((ctrl.speed - 2) * dt);

			//std::cout << counter << std::endl
			//counter++;
			//	Logger::critical("infinite loop");
		} while (!(glm::distance(glm::abs(det.where), glm::abs(t.position)) >= 0 && glm::distance(glm::abs(det.where), glm::abs(t.position)) < 1));

		det.detected = false;


		GoTo(EEnemyState::Idle_, state_);

		//for (int i = 0; i < 10000; i++);
		//GoTo(EEnemyState::Patrolling, state_);*/
	}
	if (ctrl.lastState == EEnemyState::Idle_) { ctrl.lastState = EEnemyState::Chasing; GoTo(EEnemyState::Idle_, state_); }
	else { ctrl.lastState = EEnemyState::Chasing; GoTo(EEnemyState::Patrolling, state_); }
	
}

DEFAULT_EXIT(EnemyFSM, Chasing);

DEFAULT_ENTER(EnemyFSM, Idle_);

void EnemyFSM::Idle_::Run(EnemyFSM::StateComponent& state_) {

	auto& det = Engine::Registry().get<Detection>(state_.entity);
	auto& enemy = Engine::Registry().get<EnemyDescription>(state_.entity);

	if (enemy.lastState == EEnemyState::Chasing) {
		for (int i = 0; i < 10000; i++) {
			auto& animator = Engine::Registry().get<Animator>(state_.entity);
			AnimatorPlay(animator, idle_);
		}
		enemy.lastState = EEnemyState::Idle_;
		GoTo(EEnemyState::Patrolling, state_);
	}

	while (!(det.detected==true && Engine::Registry().has<CharacterController>(det.who)))  {
		auto& animator = Engine::Registry().get<Animator>(state_.entity);
		AnimatorPlay(animator, idle_);
	}

	enemy.lastState = EEnemyState::Idle_;
	GoTo(EEnemyState::Chasing, state_);
}

DEFAULT_EXIT(EnemyFSM, Idle_);
