#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct PlightSpikesStates
{
	Inactive,
	Active	
};

struct PlightSpikesFSM : public FSM<PlightSpikesStates>
{
	DEFINE_STATE(PlightSpikesFSM, PlightSpikesStates, Inactive);
	DEFINE_STATE(PlightSpikesFSM, PlightSpikesStates, Active);


	PlightSpikesFSM()
	{
		CONNECT_STATE(PlightSpikesStates, Inactive);
		CONNECT_STATE(PlightSpikesStates, Active);
	}
};
