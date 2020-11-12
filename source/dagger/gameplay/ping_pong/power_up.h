#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{
	struct PowerUp
	{
		Bool isActive{ false };

		Float32 speedMultiplier{ 2.5f };

		Float32 powerUpTime{ 6.0f };
		Float32 timeLeft{ 6.0f };

		UInt8 targetHits{ 4 };
		UInt8 currentHits{ 0 };
	};


	class PowerUpSystem : public System
	{
	public:

		inline String SystemName() { return "Power Up System "; }

		void Run() override;

	};
};