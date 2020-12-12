#include "physics.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/movement.h"

using namespace team_game;

void PhysicsSystem::Run()
{
	auto characters = Engine::Registry().view<MovableBody, Transform>();
	auto staticBodies = Engine::Registry().view<StaticBody, Transform>();

	for (auto character : characters)
	{
		auto& movableBody = characters.get<MovableBody>(character);
		auto& movablePos = characters.get<Transform>(character);


		for (auto staticBody : staticBodies)
		{
			auto& collider = staticBodies.get<StaticBody>(staticBody);
			auto& staticPos = staticBodies.get<Transform>(staticBody);

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

			// TODO - There is an edge case, unlikely to happen, but look into it
		}
	}
}
