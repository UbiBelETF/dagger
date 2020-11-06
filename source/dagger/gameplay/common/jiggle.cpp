
#include "jiggle.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void JiggleSystem::Run()
{
    Engine::Registry().view<Sprite>().each([&](Sprite& sprite)
    {
        sprite.position.x += ((rand() % 3) - 1) * 0.001f;
        sprite.position.y += ((rand() % 3) - 1) * 0.001f;

        sprite.color.r += ((rand() % 3) - 1) * 0.01f;
        sprite.color.g += ((rand() % 3) - 1) * 0.01f;
        sprite.color.b += ((rand() % 3) - 1) * 0.01f;
    });
}

