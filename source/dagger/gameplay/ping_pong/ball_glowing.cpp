#include "ball_glowing.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

void BallGlowingSystem::Run()
{
	auto view = Engine::Registry().view<BallGlowing, Sprite, SimpleCollision>();
	for (auto entity : view) {
		
		auto& glow = view.get<BallGlowing>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& collision = view.get<SimpleCollision>(entity);
		static bool change = false;
		if (collision.colided)
		{
			glow.isColored = true;
			collision.colided = false;
		}

		if (glow.isColored)
		{
			if (change) change = false;
			else change = true;
			change ? glow.color_for_ball={ 1,0,0,1 } : glow.color_for_ball={ 0,0,1,1 };

			sprite.color = glow.color_for_ball;
		}
	}
}
