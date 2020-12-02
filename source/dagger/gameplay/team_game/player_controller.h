#pragma once

#include "core/core.h"
#include "core/system.h"



using namespace dagger;

namespace lab
{
	struct NextLvl
    {
        int id=0;
    };

	struct PlayerCharacter
	{
		int speed{ 1 };
		int cooldown = 5;
	};

	class PlayerControllerSystem
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