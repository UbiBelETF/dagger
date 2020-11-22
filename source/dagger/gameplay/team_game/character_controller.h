#pragma once

#include "core/system.h"

using namespace dagger;

enum class ECharacterState
{
	Idle,
	Running
};

struct CharacterController
{
	ECharacterState state{ ECharacterState::Idle };
};

class CharacterControllerSystem : public System
{
public:
	String SystemName() override
	{
		return "Character Controller System";
	}
};