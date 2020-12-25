
#include "core/engine.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/key.h"
#include <core/graphics/animation.h>

using namespace dagger;

void KeySystem::Run()
{

	auto view = Engine::Registry().view<SimpleCollision, Key>();

	for (auto entity : view)
	{
		
		auto& col = view.get<SimpleCollision>(entity);
		auto& key = view.get<Key>(entity);


			if (col.colided)
			{
				if (Engine::Registry().has<CharacterController>(col.colidedWith))
				{
					key.SetPickedUp(true);
					
				}

			}
			col.colided = false;
		}
	}

		