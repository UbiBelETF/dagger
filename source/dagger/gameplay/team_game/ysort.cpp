#include "ysort.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

using namespace dagger;

void YSortingSystem::Run()
{
	Engine::Registry().view<Sprite, YSort>().each([](Sprite& sprite_, YSort)
		{
			sprite_.position.z = 500 + sprite_.position.y;
		});
}