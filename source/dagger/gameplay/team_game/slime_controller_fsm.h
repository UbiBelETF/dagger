#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"
#include "hero_controller_fsm.h"
enum struct ESlimeStates
{
	Idle,
	Running,
	Attacking,
	Dead
};



struct SlimeControllerFSM : public FSM<ESlimeStates>
{
	DEFINE_STATE(SlimeControllerFSM, ESlimeStates, Idle);
	DEFINE_STATE(SlimeControllerFSM, ESlimeStates, Running);
	DEFINE_STATE(SlimeControllerFSM, ESlimeStates, Attacking);
	DEFINE_STATE(SlimeControllerFSM, ESlimeStates, Dead);
	static bool stopAttackOnNextRepeat;
	static FacingPostion facingPosition;

	SlimeControllerFSM()
	{
		CONNECT_STATE(ESlimeStates, Idle);
		CONNECT_STATE(ESlimeStates, Running);
		CONNECT_STATE(ESlimeStates, Attacking);
		CONNECT_STATE(ESlimeStates, Dead);
	}
};

