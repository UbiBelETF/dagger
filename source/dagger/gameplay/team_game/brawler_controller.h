#pragma once
#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/controller_fsm.h"
#include "gameplay/team_game/animations_fsm.h"
using namespace dagger;

namespace team_game
{
	struct BrawlerCharacter
	{
		Vector2 speed{ 1,50 };
		bool double_jump{ false };
		bool jump{ false };
		bool attack{ false };
		Float32 attack_time{ 0 };
		bool run{ false };
		Float32 jump_time{ 0 };
		bool fall{ false };
		Float32 fall_time{ 0 };
	};

	class BrawlerControllerSystem : public System
	{
		ControllerFSM FSMcontroller;
		AnimationsFSM FSManimator;
	public:
		String SystemName() override {
			return "Controller System";
		}
		void Run() override;
	};
}
