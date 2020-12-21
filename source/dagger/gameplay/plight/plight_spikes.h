#pragma once

#include "core/core.h"
#include "core/system.h"

#include "gameplay/plight/plight_spikes_fsm.h"

using namespace dagger;

namespace plight
{
	struct PlightSpikes
	{
		bool activated = false;

		Float32 spikesResetTimer{ 4.f };
		Float32 currentResetTimer{ 0.f };

		Float32 animationTimer{ 0.60f };
		Float32 currentAnimationTimer{ 0.f };
	};

	class PlightSpikesSystem
		: public System
	{
		PlightSpikesFSM spikesFSM;

	public:
		String SystemName() override {
			return "Plight Spikes System";
		}

		void Run() override;
	};
}