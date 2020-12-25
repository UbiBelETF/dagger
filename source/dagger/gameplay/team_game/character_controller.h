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
	Float32 speed{ 70 };
	ECharacterShape shape{ ECharacterShape::Hero };
	Bool animationTrigger{ false };
	Bool canMove{ true };
	Bool hasKey{ false };
	String idle { "among_them_animations:knight_idle" };
	String running{ "among_them_animations:knight_run" };

	void SetShape(ECharacterShape shape_)
	{
		animationTrigger = true;
		shape = shape_;
	}
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

public:
	inline String SystemName() override { return "Character Controller System"; }

	void Run() override;

	
};