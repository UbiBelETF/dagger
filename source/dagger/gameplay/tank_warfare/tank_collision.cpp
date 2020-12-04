#include "tank_collision.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/tank_controller.h"


using namespace dagger;
using namespace tank_warfare;

void TankCollisionSystem::Run()
{
	auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();
	auto view = Engine::Registry().view<TankCharacter, Transform, SimpleCollision>();

	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& col = view.get<SimpleCollision>(entity);
		auto& tank = view.get<TankCharacter>(entity);

		if (col.colided)
		{
			if (Engine::Registry().valid(col.colidedWith))
			{
				auto& collision = viewCollisions.get<SimpleCollision>(col.colidedWith);
				auto& transform = viewCollisions.get<Transform>(col.colidedWith);

				Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

				do
				{
					Float32 dt = Engine::DeltaTime();
					if (collisionSides.x > 0)
					{
						t.position.x -= tank.speed * dt;
						t.position.z = 1;
					}
					if (collisionSides.x < 0)
					{
						t.position.x += tank.speed * dt;
						t.position.z = 1;
					}
					if (collisionSides.y > 0)
					{
						t.position.y -= tank.speed * dt;
						t.position.z = 1;
					}
					if (collisionSides.y < 0)
					{
						t.position.y += tank.speed * dt;
						t.position.z = 3;
					}
				} while (col.IsCollided(t.position, collision, transform.position));
			}
		}
	}
}