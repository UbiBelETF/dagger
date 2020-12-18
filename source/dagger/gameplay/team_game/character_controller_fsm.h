#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct ECharacterState
{
	Idle,
	Running,
	Dying,
	GetHit
};

struct CharacterFSM : public FSM<ECharacterState>
{
	DEFINE_STATE(CharacterFSM, ECharacterState, Idle);
	DEFINE_STATE(CharacterFSM, ECharacterState, Running);
	DEFINE_STATE(CharacterFSM, ECharacterState, Dying);
	DEFINE_STATE(CharacterFSM, ECharacterState, GetHit);

	CharacterFSM()
	{
		CONNECT_STATE(ECharacterState, Idle);
		CONNECT_STATE(ECharacterState, Running);
		CONNECT_STATE(ECharacterState, Dying);
		CONNECT_STATE(ECharacterState, GetHit);
	}
};
