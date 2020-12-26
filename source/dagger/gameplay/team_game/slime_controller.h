#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/slime_controller_fsm.h"
#include <deque>

using namespace dagger;

namespace team_game
{
	struct TeamGameSlime
	{
		int speed{ 20 };
		String type = "normal";
	};
	enum Movement{STAY,LEFT,RIGHT,UP,DOWN};
	struct SlimeAiCommand {
		Movement move;
		bool attack=false;
	};
	struct SlimeAi {
		SlimeAiCommand current;
		std::deque<SlimeAiCommand>previous;
	};
	class TeamGameSlimeControllerSystem
		: public System
	{
		SlimeControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Team Game Slime Controller System";
		}

		void Run() override;

	};


}