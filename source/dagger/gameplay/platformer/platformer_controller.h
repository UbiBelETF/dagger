#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		int speed{ 1 };
		
		bool isRolling{ false };
		Float32 rollingSpeed{ 25.0f };
		Float32 rollingTime{ 0.75f };
		Float32 timeRolling{ 0.0f };
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