#pragma once
#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{
	struct PowerUp
	{
		Bool is_active{ false };

		Float32 speed_multiplier{ 2.5f };

		Float32 power_up_time{ 6.0f };
		Float32 time_left{ 6.0f };

		UInt8 target_hits{ 4 };
		UInt8 current_hits{ 0 };
	};


	class PowerUpSystem : public System
	{
	public:

		inline String SystemName() { return "Power Up System "; }

		void Run() override;

	};
};