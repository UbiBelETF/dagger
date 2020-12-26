#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/hero_controller.h"
#include "gameplay/team_game/hero_controller_fsm.h"
#include "attack_system.h"
using namespace dagger;

// Idle
using namespace team_game;
bool HeroControllerFSM::stopAttackOnNextRepeat = false;
FacingPostion HeroControllerFSM::facingPosition = side;

void HeroControllerFSM::Idle::Enter(HeroControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "chara_hero:hero_idle");
}

void HeroControllerFSM::Idle::Run(HeroControllerFSM::StateComponent& state_)
{
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);

	if (EPSILON_NOT_ZERO(input.Get("run-left-right")) || EPSILON_NOT_ZERO(input.Get("run-up-down")) && !EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(EHeroStates::Running, state_);
	}

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(EHeroStates::Attacking, state_);
	}
}

DEFAULT_EXIT(HeroControllerFSM, Idle);

void HeroControllerFSM::Running::Enter(HeroControllerFSM::StateComponent& state_)
{}

void HeroControllerFSM::Running::Run(HeroControllerFSM::StateComponent& state_)
{
	auto&& [transform, sprite, input, character,attackPlayer] = Engine::Registry().get<Transform, Sprite, InputReceiver, team_game::TeamGameCharacter,Attack>(state_.entity);
	auto& animator = Engine::Registry().get<Animator>(state_.entity);

	Float32 run_left_right = input.Get("run-left-right");
	Float32 run_up_down = input.Get("run-up-down");

	if (EPSILON_ZERO(run_left_right) && EPSILON_ZERO(run_up_down))
	{
		if(!EPSILON_NOT_ZERO(input.Get("attack")))
			GoTo(EHeroStates::Idle, state_);
	}
	else
	{
		if (!EPSILON_ZERO(run_up_down) && !EPSILON_ZERO(run_left_right))
		{
			HeroControllerFSM::facingPosition = side;
			sprite.scale.x = run_left_right * abs(sprite.scale.x);
			Vector3 normalized_vector = glm::normalize(Vector3(run_left_right, run_up_down, 0));
			transform.position += Vector3(normalized_vector.x * character.speed, normalized_vector.y * character.speed, 0)  * Engine::DeltaTime();
		}

		else if (!EPSILON_ZERO(run_left_right))
		{
			HeroControllerFSM::facingPosition = side;
			sprite.scale.x = run_left_right * abs(sprite.scale.x);;
			transform.position.x += character.speed * run_left_right * Engine::DeltaTime();
		}
		
		else if (!EPSILON_ZERO(run_up_down))
		{
			transform.position.y += character.speed * run_up_down * Engine::DeltaTime();

			if(run_up_down > 0)
				HeroControllerFSM::facingPosition = up;
			else
				HeroControllerFSM::facingPosition = down;


		}
		
	}
	
	if (run_up_down > 0) {
		AnimatorPlay(animator, "chara_hero:hero_move_up");
		attackPlayer.offsetVec = Vector3(0, 10,0);
	}

	else if (run_up_down < 0) {
		AnimatorPlay(animator, "chara_hero:hero_move_down");
		attackPlayer.offsetVec = Vector3(0, -10, 0);
	}

	else if (run_left_right != 0) {
		AnimatorPlay(animator, "chara_hero:hero_move_side");
		attackPlayer.offsetVec = Vector3(run_left_right*10, 0, 0);
	}

	if (EPSILON_NOT_ZERO(input.Get("attack")))
	{
		GoTo(EHeroStates::Attacking, state_);
	}
}

void HeroControllerFSM::Running::Exit(HeroControllerFSM::StateComponent& state_)
{}

void HeroControllerFSM::Attacking::Enter(HeroControllerFSM::StateComponent& state_)
{
	HeroControllerFSM::stopAttackOnNextRepeat = false;

	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto&& attackPlayer = Engine::Registry().get<Attack>(state_.entity);
	if(HeroControllerFSM::facingPosition == side)
		AnimatorPlay(animator, "chara_hero:hero_attack_side");

	if (HeroControllerFSM::facingPosition == up)
		AnimatorPlay(animator, "chara_hero:hero_attack_up");

	if (HeroControllerFSM::facingPosition == down)
		AnimatorPlay(animator, "chara_hero:hero_attack_down");
	attackPlayer.finished = true;
}

void HeroControllerFSM::Attacking::Run(HeroControllerFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	auto&&  attackPlayer = Engine::Registry().get<Attack>(state_.entity);
	if (animator.currentFrame > 0)
		HeroControllerFSM::stopAttackOnNextRepeat = true;

	else if (HeroControllerFSM::stopAttackOnNextRepeat && animator.currentFrame == 0) {
		attackPlayer.finished = false;
		GoTo(EHeroStates::Idle, state_);
		
	}
}

DEFAULT_EXIT(HeroControllerFSM, Attacking);