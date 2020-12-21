#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"
#include "gameplay/team_game/animations_fsm.h"
namespace team_game {
	enum struct ECharacterStates
	{
		Idle,
		Running,
		InAir,
		Attacking,
		Interact
	};

	struct ControllerFSM : public FSM<ECharacterStates>
	{
		AnimationsFSM animationsFSM;
		DEFINE_STATE(ControllerFSM, ECharacterStates, Idle);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Running);
		DEFINE_STATE(ControllerFSM, ECharacterStates, InAir);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Attacking);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Interact);


		ControllerFSM()
		{
			CONNECT_STATE(ECharacterStates, Idle);
			CONNECT_STATE(ECharacterStates, Running);
			CONNECT_STATE(ECharacterStates, InAir);
			CONNECT_STATE(ECharacterStates, Attacking);
			CONNECT_STATE(ECharacterStates, Interact);
		}
		void Run(StateComponent& component_) override;

	};
}