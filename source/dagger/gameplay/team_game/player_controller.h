#pragma once

#include "core/core.h"
#include "core/system.h"
#include "tile.h"
#include "core/game/transforms.h"



using namespace dagger;

namespace lab
{
	struct NextLvl
    {
        int id=0;
		TilemapLegend legend;
    };
	void GenerateRoom(int idNext_,lab::NextLvl& currentLvl_,Transform &tr_);

	struct CollideW{
		int id=0; //id=0->wall id=1->enemy id=2->chect

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