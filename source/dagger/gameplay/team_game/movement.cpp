#include "movement.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace team_game;

void MovementSystem::Run()
{
	Engine::Registry().view<MovableBody, Transform>().each([](MovableBody& body_, Transform& transform_)
		{
			if (EPSILON_NOT_ZERO(body_.movement.x) || EPSILON_NOT_ZERO(body_.movement.y))
			{
				transform_.position.x += body_.movement.x;
				transform_.position.y += body_.movement.y;
				body_.reset();
			}
		});
}
