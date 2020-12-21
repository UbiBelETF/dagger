#include "door_interaction.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
#include "core/graphics/text.h"
#include <core/graphics/animation.h>

using namespace dagger;

void DoorSystem::Run()
{
	auto& reg = Engine::Registry();

	auto view = reg.view<Transform, SimpleCollision, Door>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& door = view.get<Door>(entity);

		if (col.colided)
		{
			if (reg.has<CharacterController>(col.colidedWith))
			{
				

					auto& animator = Engine::Registry().get<Animator>(entity);
					AnimatorPlay(animator, "among_them_animations:door_open");
					auto ui = reg.create();
					auto& text = reg.emplace<Text>(ui);
					text.spacing = 0.6f;
					text.Set("pixel-font", "hello world");
				
			}
			
		}
		col.colided = false;

	}
}