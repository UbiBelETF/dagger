#include "animation_fsm.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/animation.h"

#include "team_game_collisions.h"
#include "gravity.h"
#include "character_controller.h"

using namespace dagger;
using namespace team_game;

void team_game::AnimationFSM::Idle::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [animator,character] = Engine::Registry().get<Animator, PlayerCharacter>(state_.entity);
	if (character.id == 0)
	{
		AnimatorPlay(animator, "TeamGame:Player-Bomb_Guy:IDLE");
	}
	else
	{
		AnimatorPlay(animator, "TeamGame:Enemy-Bald_Pirate:IDLE");
	}
}

void team_game::AnimationFSM::Idle::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Idle::Exit(AnimationFSM::StateComponent& state_)
{
}


void team_game::AnimationFSM::Running::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [animator, character] = Engine::Registry().get<Animator, PlayerCharacter>(state_.entity); 
	if (character.id == 0)
	{
		AnimatorPlay(animator, "TeamGame:Player-Bomb_Guy:RUN");
	}
	else
	{
		AnimatorPlay(animator, "TeamGame:Enemy-Bald_Pirate:RUN");
	}
}

void team_game::AnimationFSM::Running::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Running::Exit(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Jumping::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [gravity, animator, collider, character] = Engine::Registry().get<Gravity, Animator, Collider, PlayerCharacter>(state_.entity);
	if (gravity.verticalCurrentSpeed <= 0 || !collider.canGoUp)
	{
		GoTo(EAnimationStates::Falling, state_);
	}
	if (character.id == 0)
	{
		AnimatorPlay(animator, "TeamGame:Player-Bomb_Guy:JUMP");
	}
	else
	{
		AnimatorPlay(animator, "TeamGame:Enemy-Bald_Pirate:JUMP");
	}
}

void team_game::AnimationFSM::Jumping::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Jumping::Exit(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Falling::Run(AnimationFSM::StateComponent& state_)
{
	auto&& [collider, animator, character] = Engine::Registry().get<Collider, Animator, PlayerCharacter>(state_.entity);
	if (!collider.canGoDown)
	{
		GoTo(EAnimationStates::Idle, state_);
	}
	if (character.id == 0)
	{
		AnimatorPlay(animator, "TeamGame:Player-Bomb_Guy:FALL");
	}
	else
	{
		AnimatorPlay(animator, "TeamGame:Enemy-Bald_Pirate:FALL");
	}
}

void team_game::AnimationFSM::Falling::Enter(AnimationFSM::StateComponent& state_)
{
}

void team_game::AnimationFSM::Falling::Exit(AnimationFSM::StateComponent& state_)
{
}