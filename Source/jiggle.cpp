
#include "jiggle.h"
#include "sprite.h"
#include "engine.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void JiggleSystem::Run()
{
    Engine::Registry().view<Sprite>().each([&](Sprite& sprite)
    {
        sprite.m_Position.x += ((rand() % 3) - 1) * 0.001f;
        sprite.m_Position.y += ((rand() % 3) - 1) * 0.001f;

        sprite.m_Color.r += ((rand() % 3) - 1) * 0.01f;
        sprite.m_Color.g += ((rand() % 3) - 1) * 0.01f;
        sprite.m_Color.b += ((rand() % 3) - 1) * 0.01f;
    });
}

