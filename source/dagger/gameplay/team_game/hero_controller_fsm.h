#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EHeroStates
{
	Idle,
	Running,
	Attacking
};

enum FacingPostion
{
	up,
	down,
	side
};

struct HeroControllerFSM : public FSM<EHeroStates>
{
	DEFINE_STATE(HeroControllerFSM, EHeroStates, Idle);
	DEFINE_STATE(HeroControllerFSM, EHeroStates, Running);
	DEFINE_STATE(HeroControllerFSM, EHeroStates, Attacking);

	static bool stopAttackOnNextRepeat;
	static FacingPostion facingPosition;
	HeroControllerFSM()
	{
		CONNECT_STATE(EHeroStates, Idle);
		CONNECT_STATE(EHeroStates, Running);
		CONNECT_STATE(EHeroStates, Attacking);
	}
};
