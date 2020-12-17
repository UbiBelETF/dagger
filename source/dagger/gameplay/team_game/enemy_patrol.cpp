#include "enemy_patrol.h"

#include "core/engine.h"
#include "core/input/inputs.h"

#include "gameplay/team_game/character.h"
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/enemy_controller.h"

using namespace team_game;

void EnemyPatrolSystem::Run()
{
	Engine::Registry().view<EnemyCharacter, ActionManager, Collider>()
		.each([](EnemyCharacter& enemyCharacter_, ActionManager& actions_, Collider& collider_)
		{
			if (collider_.listOfEntities.size() > 0)
			{
				for (UInt8 i = 0; i < collider_.listOfEntities.size(); ++i)
				{
					if (!Engine::Registry().has<PlayerCharacter>(collider_.listOfEntities[i]) 
								&& collider_.listOfCollisionSides[i] != CollisionSide::BOTTOM
								&& collider_.listOfCollisionSides[i] != CollisionSide::TOP)
					{
						actions_.run *= -1;
					}
				}
			}
		});
}
