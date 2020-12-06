#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{
	struct TankCharacter
	{
		int speed{ 150 };
		int lastOrientation{ 0 };
		int coins{ 0 };
		int fireRate{ 1 };
		Float32 powerDuration{ 0 };
		int lives{ 3 };
		Float32 health{ 100 };
		Float32 shield{ 100 };
	};

	class TankControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Tank Controller System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};
}