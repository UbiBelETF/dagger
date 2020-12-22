
#include "door_interaction.h"
#include "core/engine.h"
#include "core/game/transforms.h"
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

bool keytaken = false;

void DoorSystem::Run()
{
	
	
	auto viewkey= Engine::Registry().view<Transform, SimpleCollision, Key>();
	for (auto keyentity : viewkey)
	{
		auto& key = viewkey.get<Key>(keyentity);
		if (key.GetPickedUp() == true) {
			keytaken = true;
			Engine::Registry().destroy(keyentity);
		}

	}
	auto view = Engine::Registry().view<Transform, SimpleCollision,StaticBody,Door>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& transform = view.get<Transform>(entity);
		auto& statbody = view.get<StaticBody>(entity);

		if (col.colided)
		{
			if (Engine::Registry().has<CharacterController>(col.colidedWith))
				{
					    if (keytaken ==true) {
						statbody.enabled = false;
						auto ui = Engine::Registry().create();
						auto& text = Engine::Registry().emplace<Text>(ui);
						text.spacing = 0.6f;
						text.Set("pixel-font", "You win");
						auto& animator = Engine::Registry().get<Animator>(entity);
						AnimatorPlay(animator, "among_them_animations:door_open");
					}
			
				}

		}
		col.colided = false;

	}
}