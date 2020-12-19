#pragma once
#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/controller_fsm.h"
#include "gameplay/team_game/animations_fsm.h"
#include "gameplay/team_game/boss_fsm.h"
using namespace dagger;

namespace team_game
{
	struct BrawlerCharacter
	{
		Vector2 speed{ 1,50 };
		Bool run = false;
		Bool attack = false;
		Float32 attacking = 0.0f;
		Float32 attackSize = 50;
		UInt32 healthHearts = 3;
		Bool doubleJump = false;
		Bool hittedEnemy = false;
		Bool dead = false;
		UInt32 hitSize = 10;
		Float32 gotHit = 0.f;
		 
	};

	class BrawlerControllerSystem : public System
	{
		ControllerFSM m_ControllerFSM;
		AnimationsFSM m_AnimatorFSM;
		BossFSM m_BossFSM;
	public:
		String SystemName() override {
			return "Controller System";
		}
		void Run() override;
	};
}
