#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace plight
{
	struct TopdownCharacter
	{
		bool running{ false };
		bool resting{ false };

		int speed{ 1 };

		Float32 restingTime = 0.75f;
		Float32 currentRestingTime = 0.f;
	};

	class TopdownControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};
}