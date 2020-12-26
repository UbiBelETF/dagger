#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/slime_controller_fsm.h"
#include "gameplay/team_game/slime_controller.h"
#include "attack_system.h"
using namespace dagger;
using namespace team_game;
// Idle

bool SlimeControllerFSM::stopAttackOnNextRepeat = false;
FacingPostion SlimeControllerFSM::facingPosition = side;

void SlimeControllerFSM::Idle::Enter(SlimeControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "chara_slime:slime_idle");
}

void SlimeControllerFSM::Idle::Run(SlimeControllerFSM::StateComponent& state_)
{
	auto& slimeAi = Engine::Registry().get<SlimeAi>(state_.entity);
	if(!slimeAi.alive)GoTo(ESlimeStates::Dead, state_);
	if (slimeAi.current.move != STAY|| slimeAi.current.attack == true)
		GoTo(ESlimeStates::Running, state_);	
}

DEFAULT_EXIT(SlimeControllerFSM, Idle);

void SlimeControllerFSM::Running::Enter(SlimeControllerFSM::StateComponent& state_)
{}

void SlimeControllerFSM::Running::Run(SlimeControllerFSM::StateComponent& state_)
{
	auto&& [transform, sprite,slimeAi,slime,attackSlime] = Engine::Registry().get<Transform, Sprite, SlimeAi, TeamGameSlime,Attack>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	if (!slimeAi.alive)GoTo(ESlimeStates::Dead, state_);
	Float32 run_left_right = 0;
	Float32 run_up_down = 0;
	switch (slimeAi.current.move) {
	case LEFT:
		run_left_right = -1;
		break;
	case RIGHT:
		run_left_right = 1;
		break;
	case UP:
		run_up_down = -1;
		break;
	case DOWN:
		run_up_down = 1;
		break;

		default: break;
	}
	if (EPSILON_ZERO(run_left_right) && EPSILON_ZERO(run_up_down))
	{
		if (slimeAi.current.attack==false)
			GoTo(ESlimeStates::Idle, state_);
	}
	else
	{
		if (!EPSILON_ZERO(run_up_down) && !EPSILON_ZERO(run_left_right))
		{
			SlimeControllerFSM::facingPosition = side;
			sprite.scale.x = run_left_right * abs(sprite.scale.x);
			Vector3 normalized_vector = glm::normalize(Vector3(run_left_right, run_up_down, 0));
			transform.position += Vector3(normalized_vector.x * slime.speed, normalized_vector.y * slime.speed, 0) * Engine::DeltaTime();
		}

		else if (!EPSILON_ZERO(run_left_right))
		{
			SlimeControllerFSM::facingPosition = side;
			sprite.scale.x = run_left_right * abs(sprite.scale.x);;
			transform.position.x += slime.speed * run_left_right * Engine::DeltaTime();
		}

		else if (!EPSILON_ZERO(run_up_down))
		{
			transform.position.y += slime.speed * run_up_down * Engine::DeltaTime();

			if (run_up_down > 0)
				SlimeControllerFSM::facingPosition = up;
			else
				SlimeControllerFSM::facingPosition = down;


		}

	}
	float bigSlimeBonus = 1.0f;
	if (slime.type == "boss")bigSlimeBonus = 2.0f;
	if (run_up_down > 0) {
		AnimatorPlay(animator, "chara_slime:slime_move_up");
		attackSlime.offsetVec = Vector3(0, 10*bigSlimeBonus, 0);
	}
	else if (run_up_down < 0) {
		AnimatorPlay(animator, "chara_slime:slime_move_down");
		attackSlime.offsetVec = Vector3(0, -10 * bigSlimeBonus, 0);
	}
	else if (run_left_right != 0) {
		AnimatorPlay(animator, "chara_slime:slime_move_side");
		attackSlime.offsetVec = Vector3(10*run_left_right * bigSlimeBonus, 0, 0);

	}

	if (slimeAi.current.attack == true)
	{
		GoTo(ESlimeStates::Attacking, state_);
	}
	
}

void SlimeControllerFSM::Running::Exit(SlimeControllerFSM::StateComponent& state_)
{}

void SlimeControllerFSM::Attacking::Enter(SlimeControllerFSM::StateComponent& state_)
{
	SlimeControllerFSM::stopAttackOnNextRepeat = false;

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& slimeAi = Engine::Registry().get<SlimeAi>(state_.entity);
	auto&& attackSlime = Engine::Registry().get<Attack>(state_.entity);
	if (SlimeControllerFSM::facingPosition == side)
		AnimatorPlay(animator, "chara_slime:slime_attack_side");

	if (SlimeControllerFSM::facingPosition == up)
		AnimatorPlay(animator, "chara_slime:slime_attack_up");

	if (SlimeControllerFSM::facingPosition == down)
		AnimatorPlay(animator, "chara_slime:slime_attack_down");
	if(slimeAi.alive)
	attackSlime.finished = true;
}

void SlimeControllerFSM::Attacking::Run(SlimeControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto& slimeAi = Engine::Registry().get<SlimeAi>(state_.entity);
	auto& attackSlime = Engine::Registry().get<Attack>(state_.entity);
	if (animator.currentFrame > 0)
		stopAttackOnNextRepeat = true;

	 if ((SlimeControllerFSM::stopAttackOnNextRepeat && animator.currentFrame == 0) || (!slimeAi.alive)) {
		attackSlime.finished = false;
		GoTo(ESlimeStates::Idle, state_);
	}
}

DEFAULT_EXIT(SlimeControllerFSM, Attacking);
void SlimeControllerFSM::Dead::Enter(SlimeControllerFSM::StateComponent& state_) {
	auto& attackSlime = Engine::Registry().get<Attack>(state_.entity);
	attackSlime.finished = false;
	auto&& sprite = Engine::Registry().get< Sprite>(state_.entity);
	AssignSprite(sprite, "spritesheets:tiles_dungeon:empty");
}
void SlimeControllerFSM::Dead::Run(SlimeControllerFSM::StateComponent& state_) {

	auto&& sprite = Engine::Registry().get< Sprite>(state_.entity);
	AssignSprite(sprite, "spritesheets:tiles_dungeon:empty");
}
void SlimeControllerFSM::Dead::Exit(SlimeControllerFSM::StateComponent& state_) {
	auto&& sprite = Engine::Registry().get< Sprite>(state_.entity);
	AssignSprite(sprite, "spritesheets:tiles_dungeon:empty");
}