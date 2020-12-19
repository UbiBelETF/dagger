#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{

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