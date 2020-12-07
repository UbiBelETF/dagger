#include "animation_fsm.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"

using namespace dagger;
using namespace team_game;

void team_game::AnimationFSM::Idle::Run(AnimationFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "TeamGame:Player-Bomb_Guy:IDLE");
}

void team_game::AnimationFSM::Idle::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Idle::Exit(AnimationFSM::StateComponent& state_)
{
}
