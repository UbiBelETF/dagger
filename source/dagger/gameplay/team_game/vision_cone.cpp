#include "vision_cone.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"

using namespace dagger;
using namespace team_game;

void VisionConeSystem::Run()
{
	auto view = Engine::Registry().view<CharacterController>();
	auto cones = Engine::Registry().view<VisionCone, Sprite>();
	auto ctrlEntity = view.begin();

	for (auto entity : cones)
	{
		auto& controller = Engine::Registry().get<CharacterController>(*ctrlEntity);
		auto& sprite = Engine::Registry().get<Sprite>(entity);
		auto& cone = Engine::Registry().get<VisionCone>(entity);

		if (controller.shape == cone.shape)
		{
			sprite.color.a = 0;
		}
		else
		{
			sprite.color.a = 1;
		}
	}
}
