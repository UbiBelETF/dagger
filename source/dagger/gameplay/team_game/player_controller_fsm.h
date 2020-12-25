#pragma once

#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/tile.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/character_controller_fsm.h"


using namespace dagger;

namespace lab
{
	struct NextLvl
    {
        int id=0;
		TilemapLegend legend;
		Room room;
		Room addons;
    };
	void GenerateRoom(int idNext_,lab::NextLvl& currentLvl_,Transform &tr_);

	struct CollideW{
		int id=0; //id=0->wall id=1->enemy id=2->chect

	};

	struct PlayerCharacter
	{
		int speed{ 1 };
		int maxCooldown = 35;
		int cooldown = 35;
		int health = 100;
		int maxHealth = 100;
		int deathCooldown=200;
		int hitCooldown=30;
	};


	class PlayerControllerSystem
		: public System
	{
        CharacterFSM characterFSM;

	public:
		String SystemName() override {
			return "Character Controller System";
		}		
		void Run() override;
	};
}