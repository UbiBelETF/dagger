#pragma once

#include "core/system.h"
#include "core/input/inputs.h"
#include "gameplay/common/fsm.h"
#include "core/game/transforms.h"

#include <utility>

using namespace dagger;

namespace team_game
{
	enum class ECharacterState
	{
		Idle,
		Running,
		Dashing,
		Jumping,
		Falling,
		DoubleJumping,
		Using,
		Attacking,
		Tossing,
		Dead
	};

	struct CharacterController : public FiniteStateComponent<ECharacterState>
	{
		ViewPtr<InputReceiver> userInput;
		ViewPtr<Transform> transform;

		Float32 speed{ 100 };
	};

	struct CharacterControllerFSM : public FiniteStateMachine<ECharacterState, CharacterController>
	{

		struct IdleState : public State
		{
			IdleState(CharacterControllerFSM* fsm_) : State{ fsm_ } {}

			void Run(CharacterController& ctrl_) override;
		};

		struct RunningState : public State
		{
			RunningState(CharacterControllerFSM* fsm_) : State{ fsm_ } {}

			void Run(CharacterController& ctrl_) override;
		};

		CharacterControllerFSM()
		{
			MAP_STATE_TO_CLASS(ECharacterState::Idle, IdleState);
			MAP_STATE_TO_CLASS(ECharacterState::Running, RunningState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Dashing, DashingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Jumping, JumpingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Falling, FallingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::DoubleJumping, DoubleJumpingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Using, UsingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Attacking, AttackingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Tossing, TossingState);
			//		MAP_STATE_TO_CLASS(ECharacterState::Dead, DeadState);
		}
	};

	class CharacterControllerSystem : public System
	{

		CharacterControllerFSM m_CharStateMachine;

	public:
		inline String SystemName() override { return "Character Controller System"; }

		void Run() override;
	};
};