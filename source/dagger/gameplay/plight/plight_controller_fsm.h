#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct PlightCharacterStates
{
	Idle,
	Running,
	Dashing,
	Hit
};

struct PlightCharacterControllerFSM : public FSM<PlightCharacterStates>
{
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Idle);
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Running);
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Dashing);
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Hit);

	PlightCharacterControllerFSM()
	{
		CONNECT_STATE(PlightCharacterStates, Idle);
		CONNECT_STATE(PlightCharacterStates, Running);
		CONNECT_STATE(PlightCharacterStates, Dashing);
		CONNECT_STATE(PlightCharacterStates, Hit);
	}
};
