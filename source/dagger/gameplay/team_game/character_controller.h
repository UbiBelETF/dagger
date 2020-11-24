#pragma once

#include "core/system.h"

#include <utility>

using namespace dagger;

enum class ECharacterState
{
	Idle = 0,
	Running
};

struct CharacterController
{
	ECharacterState state{ ECharacterState::Idle };
	Vector2 direction{ 0, 0 };
	Float32 speed{ 100 };
	static int activeStateCount[];

	CharacterController()
	{
		activeStateCount[(int)ECharacterState::Idle]++;
	}

	void ChangeState(ECharacterState newState_);
};

class CharacterControllerSystem : public System
{
	std::map<ECharacterState, System*> m_Systems{};

public:
	template<typename Sys, typename... Args>
	inline void AddSystem(ECharacterState index_, Args&&... args_)
	{
		auto sys = new Sys(std::forward<Args>(args_)...);
		m_Systems[index_] = sys;
		CharacterController::activeStateCount[(int)index_] = 0;
	}

	String SystemName() override
	{
		return "Character Controller System";
	}

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
