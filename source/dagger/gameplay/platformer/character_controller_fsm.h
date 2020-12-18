#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace platformer
{
	enum struct EPlatformerCharacterStates
	{
		Idle,
		Running
	};

	struct PlatformerCharacterControllerFSM : public FSM<EPlatformerCharacterStates>
	{
		DEFINE_STATE(PlatformerCharacterControllerFSM, EPlatformerCharacterStates, Idle);
		DEFINE_STATE(PlatformerCharacterControllerFSM, EPlatformerCharacterStates, Running);

		PlatformerCharacterControllerFSM()
		{
			CONNECT_STATE(EPlatformerCharacterStates, Idle);
			CONNECT_STATE(EPlatformerCharacterStates, Running);
		}
	};
}