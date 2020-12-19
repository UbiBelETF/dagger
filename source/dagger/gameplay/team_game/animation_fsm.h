#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace team_game
{
	enum class EAnimationStates
	{
		Idle,
		Running,
		Jumping,
		Falling
	};

	struct AnimationFSM : public FSM<EAnimationStates>
	{
		DEFINE_STATE(AnimationFSM, EAnimationStates, Idle);
		DEFINE_STATE(AnimationFSM, EAnimationStates, Running);
		DEFINE_STATE(AnimationFSM, EAnimationStates, Jumping);
		DEFINE_STATE(AnimationFSM, EAnimationStates, Falling);

		AnimationFSM()
		{
			CONNECT_STATE(EAnimationStates, Idle);
			CONNECT_STATE(EAnimationStates, Running);
			CONNECT_STATE(EAnimationStates, Jumping);
			CONNECT_STATE(EAnimationStates, Falling);
		}
	};
}
