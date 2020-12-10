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
		Bool run = false;
	};

	class BrawlerControllerSystem : public System
	{
		ControllerFSM m_ControllerFSM;
		AnimationsFSM m_AnimatorFSM;
	public:
		String SystemName() override {
			return "Controller System";
		}
		void Run() override;
	};
}
