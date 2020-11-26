#pragma once

#include "core/system.h"
#include "gameplay/common/fsm.h"
#include "core/game/transforms.h"

#include <utility>

using namespace dagger;

// so first we need to define an enum...
enum class ECharacterState
{
	Idle,
	Running
};

// ...and some data (we call it internal if we're using it with the FSM)
// note: we actually never use this ourselves. we just pass it to the FSM, which
// extends it and uses _that_ in all the methods (called StateComponent)
struct CharacterController : public FiniteStateComponent<ECharacterState>
{
	Vector2 userInput;
	ViewPtr<Transform> transform;
	Vector2 direction{ 0, 0 };
	Float32 speed{ 100 };
};

// we now create our FSM by extending the FiniteStateMachine class and passing in our enum and data!
struct CharacterControllerFSM : public FiniteStateMachine<ECharacterState, CharacterController>
{
	// we now define a state sub-class for every state. 
	// it can have Enter(), Run() and Exit() functions, but here we need only Run()
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

	// and very importantly, we add a constructor that maps every enum field to a class.
	// if you don't do this, it WILL fail!
	CharacterControllerFSM()
	{
		MAP_STATE_TO_CLASS(ECharacterState::Idle, IdleState);
		MAP_STATE_TO_CLASS(ECharacterState::Running, RunningState);
	}
};

// now we create a system...
class CharacterControllerSystem : public System
{
	// and add the fsm as a field
	CharacterControllerFSM m_CharStateMachine;

public:
	inline String SystemName() override { return "Character Controller System"; }

	void Run() override;
};