#pragma once

#include "core/system.h"
#include "core/game/finite_state_machine.h"

using namespace dagger;

enum struct ECharacterState
{
	Idle,
	Running
};

enum struct ECharacterShape {
	Hero,
	Goblin,
	Slime,
	Bat
};

struct CharacterController
{
	Float32 speed{ 100 };
	
};

struct CharacterFSM : public FSM<ECharacterState>
{
	DEFINE_STATE(CharacterFSM, ECharacterState, Idle);
	DEFINE_STATE(CharacterFSM, ECharacterState, Running);

	CharacterFSM()
	{
		CONNECT_STATE(ECharacterState, Idle);
		CONNECT_STATE(ECharacterState, Running);
	}
};

class CharacterControllerSystem : public System
{
	CharacterFSM m_CharStateMachine;
	ECharacterShape s_shape;

public:
	inline String SystemName() override { return "Character Controller System"; }

	ECharacterShape GetShape() { return s_shape; }
	void SetShape(ECharacterShape newShape_) { s_shape = newShape_; }

	void Run() override;

	
};