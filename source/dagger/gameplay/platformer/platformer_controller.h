#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/graphics/sprite.h"

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		int speed{ 1 };
		
		bool isRolling{ false };
		bool isJumping{ false };
		bool isRunning{ false };
		bool reachedMax{ false };
		Float32 maxHeight{ 40.0f };
		Float32 rollingSpeed{ 25.0f };
		Float32 rollingTime{ 0.75f };
		Float32 timeRolling{ 0.0f };
		Float32 timeJumping{ 0.0f };
		Float32 verticalInitialSpeed{ 100 };
		Float32 gravity{ 150 };
	};

	class PlatformerControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};

	Float32 CalculateVerticalSpeed(Float32 speed_, Float32 acceleration_, Float32 time_);

	void AlignCharacter(Float32 run_, Sprite& sprite_);
}