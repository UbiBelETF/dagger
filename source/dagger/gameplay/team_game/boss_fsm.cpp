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
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "boss:boss_idle");
}
void BossFSM::Idle::Run(BossFSM::StateComponent& state_) {
	auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
	if (EPSILON_NOT_ZERO(input.Get("light"))) GoTo(EBossState::Attack, state_);
	else if (EPSILON_NOT_ZERO(input.Get("run"))) GoTo(EBossState::Running, state_);

}
DEFAULT_EXIT(BossFSM, Idle);


//running
void BossFSM::Running::Enter(BossFSM::StateComponent& state_) {
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_walk");
}
void BossFSM::Running::Run(BossFSM::StateComponent& state_) {
	auto&& [sprite, input, character,physics] = Engine::Registry().get<Sprite, InputReceiver, team_game::BrawlerCharacter,team_game::Physics>(state_.entity);
	if (EPSILON_NOT_ZERO(input.Get("light"))) GoTo(EBossState::Attack, state_);
	else if (EPSILON_ZERO(input.Get("run")))
	{
		GoTo(EBossState::Idle, state_);
	}
	else
	{
		sprite.scale.x = input.Get("run");
		physics.velocity.x += character.speed.x * sprite.scale.x ;
	}
}
DEFAULT_EXIT(BossFSM, Running);

//attack
void BossFSM::Attack::Enter(BossFSM::StateComponent& state_) {
	auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator_, "boss:boss_attack");
}
void BossFSM::Attack::Run(BossFSM::StateComponent& state_)
{
	auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
	auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
	if (EPSILON_ZERO(char_.attacking))
	{
		char_.attacking = 1.11f;
	}
	else
	{
		char_.attacking -= Engine::DeltaTime();
		if (char_.attacking <= 0)
		{
			char_.attacking = 0.f;
			if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(EBossState::Running, state_);
			else GoTo(EBossState::Idle, state_);
		}
	}
}
DEFAULT_EXIT(BossFSM, Attack);

