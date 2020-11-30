#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EHeroStates
{
	Idle,
	Running
};

struct HeroControllerFSM : public FSM<EHeroStates>
{
	DEFINE_STATE(HeroControllerFSM, EHeroStates, Idle);
	DEFINE_STATE(HeroControllerFSM, EHeroStates, Running);




	HeroControllerFSM()
	{
		CONNECT_STATE(EHeroStates, Idle);
		CONNECT_STATE(EHeroStates, Running);
	}
};
