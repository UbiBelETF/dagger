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
		bool dashing{ false };
		bool hit{ false };
		bool doubleTap{ false }; // Waiting for a double tap to perform dash animation
		bool resting{ false };
		bool dead{ false };

		Float32 speed{ 100.f };

		Float32 restingTime = 0.75f;
		Float32 currentRestingTime = 0.f;
		
		Float32 firstMoveInput{ 0.f };

		//dash
		Float32 dashingTime{ 0.50f };
		Float32 dashingSpeed{ 200.f };
		Float32 currentDashingTime{ 0.f };
		Float32 doubleTapDurationWindow{ 0.25f };
		Float32 currentDoubleTapDuration{ 0.f };

		//used for reseting characters
		Vector2 startPosition = { 0.f,0.f };

		//Hit
		Float32 hitTime{ 0.50f };
		Float32 currentHitTime{ 0.f };

		String playerNumber = "";

		Entity weaponSprite;
		Float32 weaponOffset = 10.f;
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