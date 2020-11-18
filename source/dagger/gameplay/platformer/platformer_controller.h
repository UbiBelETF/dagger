#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace platformer
{
	struct PlatformerCharacter
	{
		int speed{ 1 };
		bool jumping;
		int jumpDuration = 0;
		int maxHeight = 60;
	};

	class PlatformerControllerSystem
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