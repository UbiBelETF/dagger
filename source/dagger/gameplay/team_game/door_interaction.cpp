
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



void DoorSystem::Run()
{

	auto view = Engine::Registry().view<Detection, SimpleCollision,StaticBody,Door>();

	auto viewkey = Engine::Registry().view<Key>();
	for (auto keyentity : viewkey)
	{
		auto& key = viewkey.get<Key>(keyentity);
		if (key.GetPickedUp() == true) {
			SetPickedUp(true);
			Engine::Registry().destroy(keyentity);
		}
		else {
			SetPickedUp(false);
		}
	}

	for (auto entity : view)
	{   

		auto& col = view.get<SimpleCollision>(entity);
		auto& detect = view.get<Detection>(entity);
		auto& statbody = view.get<StaticBody>(entity);

	     	if (Engine::Registry().has<CharacterController>(detect.who)) {

			
			if (col.colided) {

				if (Engine::Registry().has<CharacterController>(col.colidedWith)) {
				       
					    auto& hero = Engine::Registry().get<CharacterController>(detect.who);

					    if (GetPickedUpp() == true)
						{
						statbody.enabled = false;
						auto ui = Engine::Registry().create();
						auto& text = Engine::Registry().emplace<Text>(ui);
						text.spacing = 1.0f;
						text.Set("pixel-font", "You win");
						auto ui2 = Engine::Registry().create();
						auto& text2 = Engine::Registry().emplace<Text>(ui2);
						text2.spacing = 1.0f;
						text2.Set("pixel-font", "Press R to restart", { 0.0f,-100.0f,0.0f });
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