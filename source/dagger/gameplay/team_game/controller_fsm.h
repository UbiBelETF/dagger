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
	};

	struct ControllerFSM : public FSM<ECharacterStates>
	{
		AnimationsFSM animationsFSM;
		DEFINE_STATE(ControllerFSM, ECharacterStates, Idle);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Running);
		DEFINE_STATE(ControllerFSM, ECharacterStates, InAir);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Attacking);


		ControllerFSM()
		{
			CONNECT_STATE(ECharacterStates, Idle);
			CONNECT_STATE(ECharacterStates, Running);
			CONNECT_STATE(ECharacterStates, InAir);
			CONNECT_STATE(ECharacterStates, Attacking);
		}
		void Run(StateComponent& component_) override;

	};
}