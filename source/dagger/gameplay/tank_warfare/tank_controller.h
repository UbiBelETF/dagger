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
		int speed{ 50 };
		ETankOrientation lastOrientation{ ETankOrientation::TankLeft };
		int health{ 100 };
		Float32 reloadTime{ 1 };
		int id{ rand() };
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