#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"
enum struct CharacterStates 
{
    Idle,
    Running,
    InAir
};

struct ControllerFSM : public FSM<CharacterStates>
{
	DEFINE_STATE(ControllerFSM, CharacterStates, Idle);
	DEFINE_STATE(ControllerFSM, CharacterStates, Running);
	DEFINE_STATE(ControllerFSM, CharacterStates, InAir);

	ControllerFSM()
	{
		CONNECT_STATE(CharacterStates, Idle);
		CONNECT_STATE(CharacterStates, Running);
		CONNECT_STATE(CharacterStates, InAir);
	}
 

};
