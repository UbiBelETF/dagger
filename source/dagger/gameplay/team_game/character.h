#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/controller_fsm.h"

using namespace dagger;

namespace team_game
{
	struct Character
	{
		Float32 health{ 100.0f };
		Float32 speed{ 70.f };
		Float32 damage{ 25.0f };
	};

	struct PlayerCharacter 
	{
		UInt8 id;
	};

	struct EnemyCharacter
	{
		UInt8 id;
	};
}
