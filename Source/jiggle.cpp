#include "jiggle.h"
#include "engine.h"

#include <algorithm>
#include <execution>

using namespace dagger;

void JiggleSystem::Run()
{
    Engine::Registry().view<Sprite>().each([&](Sprite& sprite)
    {
        sprite.m_Position.X += ((rand() % 3) - 1) * 0.1f;
        sprite.m_Position.Y += ((rand() % 3) - 1) * 0.1f;

        sprite.m_Color.R += ((rand() % 3) - 1) * 0.01f;
        sprite.m_Color.G += ((rand() % 3) - 1) * 0.01f;
        sprite.m_Color.B += ((rand() % 3) - 1) * 0.01f;
    });
}

