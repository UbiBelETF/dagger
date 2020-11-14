#include "pingpong_winner.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

using namespace dagger;

void PingPongWinnerSystem::Run()
{
	auto view = Engine::Registry().view<PingPongWinner, Sprite>();
	for (auto entity : view) {
		auto &p = view.get<PingPongWinner>(entity);
		auto &sprite = view.get<Sprite>(entity);

		if (p.playerWon)
		{
			if (p.lightLeft >= 0)
			{
				p.lightLeft -= Engine::DeltaTime();
				sprite.color = ColorRGBA(0, 1, 0, 1);
			}
			else
			{
				p.playerWon = false;
				p.lightLeft = p.lightOnTime;
				sprite.color = ColorRGBA(1, 1, 1, 1);
			}
		}
	}
}
