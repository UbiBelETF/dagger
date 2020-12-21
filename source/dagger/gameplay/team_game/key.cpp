#include "door_interaction.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/door_interaction.h"
#include "gameplay/team_game/key.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "core/graphics/text.h"
#include "core/graphics/animation.h"

using namespace dagger;

void KeySystem::Run()
{
	auto view = Engine::Registry().view<Transform, SimpleCollision, Key>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& door = view.get<Door>(entity);
		auto& key = view.get<Key>(entity);
		if (key.ispickedup == false) {

			if (col.colided)
			{
				if (Engine::Registry().has<CharacterController>(col.colidedWith))
				{
					key.SetPickedUp(true);
					auto& sprite = Engine::Registry().get<Sprite>(entity);
					AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");
				}


			}
			col.colided = false;
		}
	}

		} 



