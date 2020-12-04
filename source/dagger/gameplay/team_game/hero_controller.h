#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/hero_controller_fsm.h"


using namespace dagger;

namespace team_game
{
	struct TeamGameCharacter
	{
		int speed{ 1 };
	};

	class TeamGameControllerSystem
		: public System
	{
		HeroControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Team Game Character Controller System";
		}

		void Run() override;

	};


}
