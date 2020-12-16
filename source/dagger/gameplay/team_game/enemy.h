#pragma once

#include "core/system.h"
#include "core/game/finite_state_machine.h"

using namespace dagger;

enum struct EEnemyState
{
	Patrolling,
	Chasing
};



struct EnemyController
{
	Float32 speed{ 100 };

};

struct EnemyFSM : public FSM<EEnemyState>
{
	DEFINE_STATE(CharacterFSM, EEnemyState, Patrolling);
	DEFINE_STATE(CharacterFSM, EEnemyState, Chasing);

	EnemyFSM()
	{
		CONNECT_STATE(EEnemyState, Patrolling);
		CONNECT_STATE(EEnemyState, Chasing);
	}
};

class EnemyControllerSystem : public System
{
	EnemyFSM m_EnemyStateMachine;
	

public:
	inline String SystemName() override { return "Enemies System"; }

	void Run() override;


};