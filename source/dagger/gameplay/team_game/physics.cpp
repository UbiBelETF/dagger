#include "physics.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/movement.h"

using namespace team_game;

void PhysicsSystem::Run()
{
	auto characters = Engine::Registry().view<MovableBody, Transform>();

	for (auto character : characters)
	{
		auto& movableBody = characters.get<MovableBody>(character);
		auto& movablePos = characters.get<Transform>(character);
		
		SInt32 x = ((SInt32) movablePos.position.x + 8) / 16;
		SInt32 y = ((SInt32) movablePos.position.y - 8) / 16;

		for (SInt32 i = x - 1; i <= x + 1; i++)
		{
			for (SInt32 j = y - 1; j <= y + 1; j++)
			{
				Entity entity = Engine::GetDefaultResource<StaticBodyMap>()->get(i, j);

				if ((SInt32) entity != 0)
				{
					auto& collider = Engine::Registry().get<StaticBody>(entity);

					if (collider.enabled)
					{
						auto& staticPos = Engine::Registry().get<Transform>(entity);

						Vector2 px(movablePos.position.x - movableBody.size.x / 2.0f + movableBody.movement.x,
							movablePos.position.y - movableBody.size.y / 2.0f);
						Vector2 py(movablePos.position.x - movableBody.size.x / 2.0f,
							movablePos.position.y - movableBody.size.y / 2.0f + movableBody.movement.y);
						Vector2 p(staticPos.position.x - collider.size.x / 2.0f, staticPos.position.y - collider.size.y / 2.0f);

						Vector2 relativePos(staticPos.position.x - movablePos.position.x, staticPos.position.y - movablePos.position.y);

						if (px.x < p.x + collider.size.x &&
							px.x + movableBody.size.x > p.x &&
							px.y < p.y + collider.size.y &&
							px.y + movableBody.size.y > p.y)
						{
							if (relativePos.x > 0)
							{
								movableBody.movement.x = relativePos.x - (movableBody.size.x + collider.size.x) / 2.0f;
							}
							else
							{
								movableBody.movement.x = relativePos.x + (movableBody.size.x + collider.size.x) / 2.0f;
							}
						}
						else if (py.x < p.x + collider.size.x &&
							py.x + movableBody.size.x > p.x &&
							py.y < p.y + collider.size.y &&
							py.y + movableBody.size.y > p.y)
						{
							if (relativePos.y > 0)
							{
								movableBody.movement.y = relativePos.y - (movableBody.size.y + collider.size.y) / 2.0f;
							}
							else
							{
								movableBody.movement.y = relativePos.y + (movableBody.size.y + collider.size.y) / 2.0f;
							}
						}

						/*
							There is an edge case, unlikely to happen, when movement on either axis causes collision.
							I tried to cause it intentionally, but it is almost impossible. I have an idea how to fix it,
							but I'm putting it off until i start optimizing this system, cause i might use data needed for
							optimization to fix this problem.
						*/
					}
				}
			}
		}
	}
}
