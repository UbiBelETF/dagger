#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace topdown
{
	struct TopdownCharacter
	{
		int speed{ 1 };
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