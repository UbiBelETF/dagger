#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct CharacterStates
{
	Idle,
	Running
};

struct CharacterControllerFSM : public FSM<CharacterStates>
{
	DEFINE_STATE(CharacterControllerFSM, CharacterStates, Idle);
	DEFINE_STATE(CharacterControllerFSM, CharacterStates, Running);

	CharacterControllerFSM()
	{
		MAP_STATE_TO_CLASS(CharacterStates::Idle, Idle);
		MAP_STATE_TO_CLASS(CharacterStates::Running, Running);
	}
};
