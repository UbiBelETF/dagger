#pragma once

#include "core/system.h"

#include <utility>

using namespace dagger;

enum class ECharacterState
{
	Idle,
	Running
};

struct CharacterController
{
	ECharacterState state{ ECharacterState::Idle };
	Vector2 direction{ 0, 0 };
	Float32 speed{ 100 };
};

class CharacterControllerSystem : public System
{
	std::vector<System*> m_Systems{};

public:
	template<typename Sys, typename... Args>
	inline void AddSystem(Args&&... args_)
	{
		auto sys = new Sys(std::forward<Args>(args_)...);
//		PutDefaultResource<Sys>(sys);
		m_Systems.push_back(sys);
	}

	String SystemName() override
	{
		return "Character Controller System";
	}

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
