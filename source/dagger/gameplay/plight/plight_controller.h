#pragma once

#include "core/core.h"
#include "core/system.h"

#include "gameplay/plight/plight_controller_fsm.h"

using namespace dagger;

namespace plight
{
	struct PlightCharacterController
	{
		bool running{ false };
		bool resting{ false };

		Float32 speed{ 100.f };

		Float32 restingTime = 0.75f;
		Float32 currentRestingTime = 0.f;
		
		Float32 firstMoveInput{ 0.f };
	};

	class PlightControllerSystem
		: public System
	{
		PlightCharacterControllerFSM characterFSM;

	public:
		String SystemName() override {
			return "Plight Character Controller System";
		}

		void Run() override;
	};
}