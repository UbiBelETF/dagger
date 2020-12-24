#include "treasure.h"

#include <core/engine.h>
#include <core/game/transforms.h>
#include <core/graphics/sprite.h>

#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/gravity.h"


using namespace team_game;

void TreasureSystem::Run()
{
	auto& treasureView = Engine::Registry().view<TreasureChest>();
	for (auto& entity : treasureView)
	{
		auto& collider = Engine::Registry().get<Collider>(entity);
		if (collider.listOfEntities.size() > 0)
		{
			for (auto& ent : collider.listOfEntities)
			{
				if (Engine::Registry().has<PlayerCharacter>(ent))
				{
					auto& character = Engine::Registry().get<PlayerCharacter>(ent);
					auto& chest = Engine::Registry().get<TreasureChest>(entity);

					chest.hadCollisionWithPlayer = true;
					chest.playerId = character.id;

					break;

					Engine::Registry().remove<Sprite, Collider>(entity);
				}
			}
		}
	}
}
