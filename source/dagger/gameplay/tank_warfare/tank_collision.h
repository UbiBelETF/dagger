#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace tank_warfare
{
	class TankCollisionSystem
		: public System
	{
	public:

		inline String SystemName() { return "Tank Collision System"; }

		void Run() override;
	};
}