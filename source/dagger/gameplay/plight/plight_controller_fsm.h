#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct PlightCharacterStates
{
	Idle,
	Running
};

struct PlightCharacterControllerFSM : public FSM<PlightCharacterStates>
{
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Idle);
	DEFINE_STATE(PlightCharacterControllerFSM, PlightCharacterStates, Running);

	PlightCharacterControllerFSM()
	{
		CONNECT_STATE(PlightCharacterStates, Idle);
		CONNECT_STATE(PlightCharacterStates, Running);
	}
};
