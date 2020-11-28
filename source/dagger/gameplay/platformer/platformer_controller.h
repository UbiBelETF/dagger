#pragma once

#include "core/core.h"
#include "core/system.h"
<<<<<<< HEAD
#include "core/graphics/sprite.h"
=======
#include "gameplay/platformer/character_controller_fsm.h"
>>>>>>> main

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		UInt8 id{ 0 };

		Bool isRolling{ false };
		Bool isJumping{ false };
		Bool reachedMax{ false };
		Bool turningDuringJump{ false };
		Bool runningJump{ false };

		Bool canGoRight{ true };
		Bool canGoLeft{ true };
		Bool canGoUp{ true };
		Bool canGoDown{ true };

		Float32 currentElevation{ 0.0f };

		Float32 rollingSpeed{ 25.0f };
		Float32 rollingTime{ 0.75f };
		Float32 timeRolling{ 0.0f };
		Float32 timeJumping{ 0.0f };
		Float32 verticalInitialSpeed{ 100.0f };
		Float32 verticalSpeed{ 0.0f };
		Float32 gravity{ 150.0f };
		Float32 speed{ 0.0f };
	};

	class PlatformerControllerSystem
		: public System
	{
		CharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void Run() override;
	};

	Float32 CalculateVerticalSpeed(Float32 speed_, Float32 acceleration_, Float32 time_);

	void AlignCharacter(Float32 run_, Sprite& sprite_);
}