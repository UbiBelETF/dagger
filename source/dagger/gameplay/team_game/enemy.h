#pragma once

#include "core/system.h"
#include "core/game/finite_state_machine.h"

using namespace dagger;

enum struct EEnemyState
{
	Patrolling,
	Chasing
};

struct WalkingPath {
	static UInt32 numberOfPoints;
	static Sequence<Vector2> path;
};

struct EnemyDescription {

	Float32 speed{ 100 };
	

	SInt32 postition = 0;
	Vector2 direction = { -1, 0 };
	Vector2 offset = { 0,0 };


};



struct EnemyFSM : public FSM<EEnemyState>
{
	DEFINE_STATE(EnemyFSM, EEnemyState, Patrolling);
	DEFINE_STATE(EnemyFSM, EEnemyState, Chasing);

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