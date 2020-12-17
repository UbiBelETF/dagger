#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

namespace team_game
{
	struct ActionManager
	{
		Float32 run{ 1.0f };
		Float32 jump{ 0.0f };

		Bool attack{ false };
		Bool dead{ false };
	};

	enum class EEnemyStates
	{
		Idle,
		Running,
		Jumping,
		Falling,
		Attacking,
		Dead
	};

	struct EnemyControllerFSM : public FSM<EEnemyStates>
	{
		DEFINE_STATE(CharacterControllerFSM, EEnemyStates, Idle);
		DEFINE_STATE(CharacterControllerFSM, EEnemyStates, Running);

		EnemyControllerFSM()
		{
			CONNECT_STATE(EEnemyStates, Idle);
			CONNECT_STATE(EEnemyStates, Running);
		}
	};
}