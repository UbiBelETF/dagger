#include "power_up.h"

#include <core/engine.h>

#include <gameplay/common/simple_collisions.h>
#include "gameplay/ping_pong/pingpong_playerinput.h"

using namespace dagger;
using namespace ping_pong;

void PowerUpSystem::Run()
{
	auto view = Engine::Registry().view<PowerUp, SimpleCollision, ControllerMapping>();

	for (auto entity : view)
	{
		auto& power_up = view.get<PowerUp>(entity);
		auto& collision = view.get<SimpleCollision>(entity);
		auto& ctrl = view.get<ControllerMapping>(entity);

		if (collision.colided)
		{
			power_up.currentHits++;
			collision.colided = false;
		}
		
		if (power_up.isActive)
		{
			power_up.timeLeft -= dagger::Engine::Instance().DeltaTime();

			if (power_up.timeLeft <= 0)
			{
				ctrl.speedMultiplier = 1;
				power_up.currentHits = 0;

				power_up.isActive = false;
				power_up.timeLeft = power_up.powerUpTime;
			}
		}
		else if (power_up.currentHits >= power_up.targetHits)
		{
			ctrl.speedMultiplier = power_up.speedMultiplier;
			power_up.isActive = true;
		}

	}
}
