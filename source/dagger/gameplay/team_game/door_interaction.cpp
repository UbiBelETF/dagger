#include "door_interaction.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/key.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "core/graphics/text.h"
#include "core/graphics/animation.h"
#include "gameplay/team_game/door_interaction.h"

using namespace dagger;

void DoorSystem::Run()
{
	

	auto view = Engine::Registry().view<Transform, SimpleCollision, Door>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& door = view.get<Door>(entity);
		auto& key = view.get<Key>(entity);

		if (col.colided)
		{
			if (Engine::Registry().has<CharacterController>(col.colidedWith))
			{
				if (key.GetPickedUp() == false) {
					auto ui = Engine::Registry().create();
					auto& text = Engine::Registry().emplace<Text>(ui);
					text.spacing = 0.6f;
					text.Set("pixel-font", "hello world");
				}
				else {
					auto& animator = Engine::Registry().get<Animator>(entity);
					AnimatorPlay(animator, "among_them_animations:door_open");
				}
			}

			col.colided = false;
		}
	}
}