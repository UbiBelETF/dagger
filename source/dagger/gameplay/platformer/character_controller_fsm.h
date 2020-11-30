#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct ECharacterStates
{
	Idle,
	Running
};

struct CharacterControllerFSM : public FSM<ECharacterStates>
{
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Idle);
	DEFINE_STATE(CharacterControllerFSM, ECharacterStates, Running);

	CharacterControllerFSM()
	{
		CONNECT_STATE(ECharacterStates, Idle);
		CONNECT_STATE(ECharacterStates, Running);
	}
};
