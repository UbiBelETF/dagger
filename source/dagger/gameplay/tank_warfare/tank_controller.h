#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{
	enum struct ETankOrientation
	{
		TankLeft,
		TankRight,
		TankUp,
		TankDown
	};

	struct TankCharacter
	{
		int id{ rand() };
		int speed{ 50 };
		ETankOrientation lastOrientation{ ETankOrientation::TankLeft };
		int coins{ 0 };
		int fireRate{ 1 };
		int maxFireRate{ 2 };
		Float32 reloadTime{ 1 };
		Float32 powerDuration{ 0 };
		int lives{ 3 };
		Float32 health{ 100 };
		Float32 maxHealth{ 100 };
		Float32 shield{ 100 };
		Float32 maxShield{ 100 };
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