#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"


enum struct CharacterStates
{
	Idle,
	Sliding,
	Dead
};

class CharacterController : public FSM<CharacterStates>
{
public:
	DEFINE_STATE(CharacterController, CharacterStates, Idle);
	DEFINE_STATE(CharacterController, CharacterStates, Sliding);
	DEFINE_STATE(CharacterController, CharacterStates, Dead);


	CharacterController()
	{
		CONNECT_STATE(CharacterStates, Idle);
		CONNECT_STATE(CharacterStates, Sliding);
		CONNECT_STATE( CharacterStates, Dead);

	}

};
