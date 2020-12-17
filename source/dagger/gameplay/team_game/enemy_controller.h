#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/enemy_controller_fsm.h"

using namespace dagger;

namespace team_game
{
	class EnemyControllerSystem
		: public System
	{
		EnemyControllerFSM enemyFSM;

	public:
		String SystemName() override {
			return "Enemy Controller System";
		}

		void Run() override;
	};
}