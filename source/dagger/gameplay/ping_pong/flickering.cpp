#include "flickering.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>

#include "gameplay/ping_pong/power_up.h"

using namespace dagger;

void FlickeringSystem::Run()
{
	auto view = Engine::Registry().view<Flickering, PowerUp, Sprite>();

	for (auto entity : view)
	{
		auto& flickering = view.get<Flickering>(entity);
		auto& power_up = view.get<PowerUp>(entity);
		auto& sprite = view.get<Sprite>(entity);

		if (power_up.is_active)
		{
			flickering.time_left -= dagger::Engine::Instance().DeltaTime();

			if (flickering.time_left <= 0.0f)
			{
				if (flickering.is_on)
				{
					ChangeColor(sprite, flickering, false);
					flickering.is_on = false;
				}
				else
				{
					ChangeColor(sprite, flickering, true);
					flickering.is_on = true;
				}

				flickering.time_left = flickering.transition_period;
			}
		}
		else
		{
			ChangeColor(sprite, flickering, false);
			flickering.is_on = false;
		}
	}
}

void FlickeringSystem::ChangeColor(Sprite& sprite, Flickering& flicker, Bool is_on)
{
	if (is_on)
	{
		sprite.color = flicker.on_color;
	}
	else
	{
		sprite.color = flicker.off_color;
	}

}
