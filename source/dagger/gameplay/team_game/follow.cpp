#include "follow.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace team_game;

void FollowSystem::Run()
{
	Engine::Registry().view<Follow, Transform>().each([](Follow& follow_, Transform& transform_)
		{
			transform_.position = Engine::Registry().get<Transform>(follow_.target).position + follow_.offset;
		});
}
