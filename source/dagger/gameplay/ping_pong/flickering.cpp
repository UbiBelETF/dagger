#include "flickering.h"

#include <core/engine.h>
#include <core/graphics/sprite.h>

#include "gameplay/ping_pong/power_up.h"

using namespace dagger;
using namespace ping_pong;

void FlickeringSystem::Run()
{
	auto view = Engine::Registry().view<Flickering, PowerUp, Sprite>();

	for (auto entity : view)
	{
		auto& flickering = view.get<Flickering>(entity);
		auto& power_up = view.get<PowerUp>(entity);
		auto& sprite = view.get<Sprite>(entity);

		if (power_up.isActive)
		{
			flickering.timeLeft -= dagger::Engine::Instance().DeltaTime();

			if (flickering.timeLeft <= 0.0f)
			{
				if (flickering.isOn)
				{
					ChangeColor(sprite, flickering, false);
					flickering.isOn = false;
				}
				else
				{
					ChangeColor(sprite, flickering, true);
					flickering.isOn = true;
				}

				flickering.timeLeft = flickering.transitionPeriod;
			}
		}
		else
		{
			ChangeColor(sprite, flickering, false);
			flickering.isOn = false;
		}
	}
}

void FlickeringSystem::ChangeColor(Sprite& sprite_, Flickering& flickering_, Bool isOn_)
{
	if (isOn_)
	{
		sprite_.color = flickering_.onColor;
	}
	else
	{
		sprite_.color = flickering_.offColor;
	}

}
