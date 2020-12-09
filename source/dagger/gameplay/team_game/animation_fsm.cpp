#include "animation_fsm.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"

#include "team_game_collisions.h"
#include "gravity.h"

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


void team_game::AnimationFSM::Running::Run(AnimationFSM::StateComponent& state_)
{
	auto& anim = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(anim, "TeamGame:Player-Bomb_Guy:RUN");
}

void team_game::AnimationFSM::Running::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Running::Exit(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Jumping::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [gravity, anim, collider] = Engine::Registry().get<Gravity, Animator, Collider>(state_.entity);
	if (gravity.verticalCurrentSpeed <= 0 || !collider.canGoUp)
	{
		GoTo(EAnimationStates::Falling, state_);
	}
	AnimatorPlay(anim, "TeamGame:Player-Bomb_Guy:JUMP");
}

void team_game::AnimationFSM::Jumping::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Jumping::Exit(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Falling::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [collider, anim] = Engine::Registry().get<Collider, Animator>(state_.entity);
	if (!collider.canGoDown)
	{
		GoTo(EAnimationStates::Idle, state_);
	}
	AnimatorPlay(anim, "TeamGame:Player-Bomb_Guy:FALL");
}

void team_game::AnimationFSM::Falling::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Falling::Exit(AnimationFSM::StateComponent& state_)
{
}