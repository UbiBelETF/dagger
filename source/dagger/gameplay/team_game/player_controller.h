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

	struct CollideW{
		int id; //id=0->wall id=1->enemy id=2->chect
	};

	struct PlayerCharacter
	{
		int speed{ 1 };
		int maxCooldown = 20;
		int cooldown = 20;
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