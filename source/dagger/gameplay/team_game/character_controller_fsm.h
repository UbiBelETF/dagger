#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace lab
{
	enum struct ECharacterState
	{
		Idle,
		Running,
		Shooting
	};

	struct CharacterFSM : public FSM<ECharacterState>
	{
		DEFINE_STATE(CharacterFSM, ECharacterState, Idle);
		DEFINE_STATE(CharacterFSM, ECharacterState, Running);

		CharacterFSM()
		{
			CONNECT_STATE(ECharacterState, Idle);
			CONNECT_STATE(ECharacterState, Running);
		}
	};
}