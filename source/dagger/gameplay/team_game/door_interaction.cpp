
#include "door_interaction.h"
#include "core/engine.h"
#include "gameplay/team_game/detection.h"
#include "gameplay/team_game/key.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "core/graphics/text.h"
#include "core/graphics/animation.h"
#include "gameplay/team_game/door_interaction.h"
#include "gameplay/team_game/physics.h"
#include <core/graphics/animation.h>
using namespace team_game;


using namespace dagger;


Bool pickedup = false;
void DoorSystem::Run()
{

	
	
	auto view = Engine::Registry().view<Detection, SimpleCollision,StaticBody,Door>();

	auto viewkey = Engine::Registry().view<Key>();
	for (auto keyentity : viewkey)
	{
		auto& key = viewkey.get<Key>(keyentity);
		if (key.GetPickedUp() == true) {
			pickedup = true;
			Engine::Registry().destroy(keyentity);
		}
		else { pickedup = false; }

	}
	for (auto entity : view)
	{   

		auto& col = view.get<SimpleCollision>(entity);
		auto& detect = view.get<Detection>(entity);
		auto& statbody = view.get<StaticBody>(entity);

	     	if (Engine::Registry().has<CharacterController>(detect.who)) {

			
			if (col.colided)
			{

				if (Engine::Registry().has<CharacterController>(col.colidedWith))
				{

					    if (pickedup == true) {
						statbody.enabled = false;
					    auto& hero = Engine::Registry().get<CharacterController>(detect.who);
						auto ui = Engine::Registry().create();
						auto& text = Engine::Registry().emplace<Text>(ui);
						text.spacing = 0.6f;
						text.Set("pixel-font", "You win");
						auto& animator = Engine::Registry().get<Animator>(entity);
						AnimatorPlay(animator, "among_them_animations:door_open");
						hero.canMove = false;
					}

				}
				col.colided = false;
			}
		}

	}
}