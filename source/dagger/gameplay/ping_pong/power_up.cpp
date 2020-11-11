#include "power_up.h"

#include <core/engine.h>

#include <gameplay/common/simple_collisions.h>
#include "pingpong_playerinput.h"

using namespace dagger;

void PowerUpSystem::Run()
{
	auto view = Engine::Registry().view<PowerUp, SimpleCollision, ping_pong::ControllerMapping>();

	for (auto entity : view)
	{
		auto& power_up = view.get<PowerUp>(entity);
		auto& collision = view.get<SimpleCollision>(entity);
		auto& ctrl = view.get<ping_pong::ControllerMapping>(entity);

		if (collision.colided)
		{
			power_up.current_hits++;
			collision.colided = false;
		}
		
		if (power_up.is_active)
		{
			power_up.time_left -= dagger::Engine::Instance().DeltaTime();

			if (power_up.time_left <= 0)
			{
				ctrl.speed_multiplier = 1;
				power_up.current_hits = 0;

				power_up.is_active = false;
				power_up.time_left = power_up.power_up_time;
			}
		}
		else if (power_up.current_hits >= power_up.target_hits)
		{
			ctrl.speed_multiplier = power_up.speed_multiplier;
			power_up.is_active = true;
		}

	}
}
