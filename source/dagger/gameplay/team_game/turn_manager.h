#pragma once

#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct ETurnStates
{
	GameStart,
	TurnStart,
	TimedMove,
	TurnEnd,
	GameEnd
};

class TurnManagerFSM : public FSM<ETurnStates>
{
	DEFINE_STATE(TurnManagerFSM, ETurnStates, GameStart);
	DEFINE_STATE(TurnManagerFSM, ETurnStates, TurnStart);
	DEFINE_STATE(TurnManagerFSM, ETurnStates, TimedMove);
	DEFINE_STATE(TurnManagerFSM, ETurnStates, TurnEnd);
	DEFINE_STATE(TurnManagerFSM, ETurnStates, GameEnd);

	TurnManagerFSM()
	{
		CONNECT_STATE(ETurnStates, GameStart);
		CONNECT_STATE(ETurnStates, TurnStart);
		CONNECT_STATE(ETurnStates, TimedMove);
		CONNECT_STATE(ETurnStates, TurnEnd);
		CONNECT_STATE(ETurnStates, GameEnd);
	}
};
