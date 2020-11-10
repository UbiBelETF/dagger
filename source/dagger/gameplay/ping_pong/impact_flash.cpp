#include "impact_flash.h"

#include "core/engine.h"
#include <core/engine.h>
#include <core/graphics/sprite.h>

#include <gameplay/common/simple_collisions.h>

using namespace dagger;

void ImpactFlashSystem::Run()
{
	auto view = Engine::Registry().view<ImpactFlash, Sprite, SimpleCollision>();
	for (auto entity : view)
	{
		auto& flash = view.get<ImpactFlash>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& collision = view.get<SimpleCollision>(entity);

		if (collision.colided)
		{
			flash.isFlashing = true;
			collision.colided = false;
		}

		if (flash.isFlashing)
		{
			if (!flash.isWindingDown)
			{
				float newValue = sprite.color.b - (flash.flashingRate * Engine::DeltaTime());
				sprite.color.b = newValue > 0.0f ? newValue : 0.0f;
				sprite.color.g = newValue > 0.0f ? newValue : 0.0f;
				if (sprite.color.b == 0.0f)
				{
					flash.isWindingDown = true;
				}
			}
			else
			{
				float newValue = sprite.color.b + (flash.flashingRate * Engine::DeltaTime());
				sprite.color.b = newValue < 1.0f ? newValue : 1.0f;
				sprite.color.g = newValue < 1.0f ? newValue : 1.0f;
				if (sprite.color.b == 1.0f)
				{
					flash.isWindingDown = false;
					flash.isFlashing = false;
				}
			}
		}
	}
}
