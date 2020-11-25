#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{
	struct TankCharacter
	{
		int speed{ 50 };
		int lastOrientation{ 0 };
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