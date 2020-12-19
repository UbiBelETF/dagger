#include "boss_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/team_game/physics.h"

using namespace team_game;
//idle
void BossFSM::Idle::Enter(BossFSM::StateComponent& state_) {
	auto& sprite_ = Engine::Registry().get<Sprite>(state_.entity);
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "boss:boss_idle");
	//sprite_.scale.x *= -1;
}
void BossFSM::Idle::Run(BossFSM::StateComponent& state_) {
	auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
	if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(EBossState::Hitted, state_);
	else if (EPSILON_NOT_ZERO(input_.Get("light"))) GoTo(EBossState::Attack, state_);
	else if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(EBossState::Running, state_);

}
DEFAULT_EXIT(BossFSM, Idle);


//running
void BossFSM::Running::Enter(BossFSM::StateComponent& state_) {
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_walk");
}
void BossFSM::Running::Run(BossFSM::StateComponent& state_) {
	auto&& [sprite, input, char_,physics] = Engine::Registry().get<Sprite, InputReceiver, team_game::BrawlerCharacter,team_game::Physics>(state_.entity);
	if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(EBossState::Hitted, state_);
	else if (EPSILON_NOT_ZERO(input.Get("light"))) GoTo(EBossState::Attack, state_);
	else if (EPSILON_ZERO(input.Get("run")))
	{
		GoTo(EBossState::Idle, state_);
	}
	else
	{
		sprite.scale.x = input.Get("run");
		physics.velocity.x += char_.speed.x * sprite.scale.x ;
	}
}
DEFAULT_EXIT(BossFSM, Running);

//attack
void BossFSM::Attack::Enter(BossFSM::StateComponent& state_) {
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_attack2");
}
void BossFSM::Attack::Run(BossFSM::StateComponent& state_)
{
	auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
	if (EPSILON_NOT_ZERO(char_.gotHit))
	{
		GoTo(EBossState::Hitted, state_);
		char_.attacking = 0.f;
		char_.hittedEnemy = false;
	}
    else if (EPSILON_ZERO(char_.attacking))
	{
		char_.attacking = 0.5f;
	}
	else
	{
		char_.attacking -= Engine::DeltaTime();
		char_.hitSize = 100 * char_.attacking + 1;
		if (char_.attacking <= 0)
		{
			char_.hitSize = 100;
			char_.attacking = 0.f;
			char_.hittedEnemy = false;
			if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(EBossState::Running, state_);
			else GoTo(EBossState::Idle, state_);
		}
	}
}
DEFAULT_EXIT(BossFSM, Attack);

//hitted
void BossFSM::Hitted::Enter(BossFSM::StateComponent& state_)
{
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_damaged");
}
void BossFSM::Hitted::Run(BossFSM::StateComponent& state_)
{
	auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
	auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
	
	char_.gotHit -=Engine::DeltaTime();

	if (char_.gotHit <= 0) {
		char_.gotHit = 0.f;
		if (char_.healthHearts==0) GoTo(EBossState::Dead, state_);
		else if (input_.Get("light")) GoTo(EBossState::Attack, state_);
		else if (input_.Get("run"))GoTo(EBossState::Running, state_);
		else GoTo(EBossState::Idle, state_);
	}
}
DEFAULT_EXIT(BossFSM, Hitted);

//dead
void BossFSM::Dead::Enter(BossFSM::StateComponent& state_)
{
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_death");
	
}
void BossFSM::Dead::Run(BossFSM::StateComponent& state_)
{

	auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
	auto& sprite_ = Engine::Registry().get<Sprite>(state_.entity);
	if (!char_.dead)
	{
		if (EPSILON_ZERO(char_.gotHit))
		{
			char_.gotHit = 0.7f;
		}
		else
		{
			char_.gotHit -= Engine::DeltaTime();
			if (char_.gotHit <= 0)
			{
				auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
				AnimatorStop(animator_);
			}

		}
	}
}
DEFAULT_EXIT(BossFSM, Dead);
