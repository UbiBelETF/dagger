#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/controller_fsm.h"
#include "gameplay/team_game/animation_fsm.h"

using namespace dagger;

namespace team_game
{
	struct PlayerCharacter
	{
		UInt8 id;
		Float32 speed{ 200.f };
		Float32 health{ 100.0f };
	};

	class CharacterControllerSystem
		: public System
	{
		CharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};
}
