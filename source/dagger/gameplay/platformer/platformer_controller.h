#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/platformer/character_controller_fsm.h"

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		int speed{ 1 };
		bool jumping;
		int jumpDuration = 0;
		int maxHeight = 60;
	};

	class PlatformerControllerSystem
		: public System
	{
		PlatformerCharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};
}