#pragma once

#include "core/system.h"
#include "core/game/finite_state_machine.h"
#include "character_controller.h"

using namespace dagger;

enum struct EEnemyState
{
	Patrolling,
	Chasing,
	NoMore
};



struct EnemyDescription {

	Float32 speed{ 40 };
	UInt32 numberOfPoints=0;
	Sequence<Vector2> path = {};
	SInt32 postition = 0;
	Vector2 direction = { -1, 0 };
	Vector2 offset = { 0,0 };
	ECharacterShape shape;
	EEnemyState lastState;
	Vector2 detectionArea{ 80,80 };

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