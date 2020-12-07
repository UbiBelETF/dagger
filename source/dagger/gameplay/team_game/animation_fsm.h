#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace team_game
{
	enum class EAnimationStates
	{
		Idle,
		Running
	};

	struct AnimationFSM : public FSM<EAnimationStates>
	{
		DEFINE_STATE(AnimationFSM, EAnimationStates, Idle);
//		DEFINE_STATE(AnimationFSM, EAnimationStates, Running);

		AnimationFSM()
		{
			CONNECT_STATE(EAnimationStates, Idle);
//			CONNECT_STATE(EAnimationStates, Running);
		}
	};
}
