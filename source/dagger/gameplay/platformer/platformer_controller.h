#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		bool jumping{ false };
		bool falling{ false };
		bool newJump{ true };
		bool rolling{ false };
		bool doubleTap{ false }; // Waiting for a double tap to perform roll animation

		
		int speed{ 1 };
		int jumpNumber{ 0 };

		Float32 startJumpSpeed{ 50.f };
		Float32 currentJumpSpeed{ 0.f };
		Float32 gravitySpeed{ 60.f };
		Float32 rollingTime{0.75f};
		Float32 startRoll{ 0.f };
		Float32 rollingSpeed{ 100.f };
		Float32 currentRollingTime{0.f};
		Float32 doubleTapDurationWindow{ 0.25f };
		Float32 currentDoubleTapDuration{ 0.f };
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
}