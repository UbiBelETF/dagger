#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"
#include "gameplay/team_game/animations_fsm.h"
namespace team_game {
	enum struct ECharacterStates
	{
		Idle,
		Running,
		InAir
	};

	struct ControllerFSM : public FSM<ECharacterStates>
	{
		AnimationsFSM animationsFSM;
		DEFINE_STATE(ControllerFSM, ECharacterStates, Idle);
		DEFINE_STATE(ControllerFSM, ECharacterStates, Running);
		DEFINE_STATE(ControllerFSM, ECharacterStates, InAir);

		ControllerFSM()
		{
			CONNECT_STATE(ECharacterStates, Idle);
			CONNECT_STATE(ECharacterStates, Running);
			CONNECT_STATE(ECharacterStates, InAir);
		}
		void Run(StateComponent& component_) override;

	};
}