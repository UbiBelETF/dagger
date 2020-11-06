#include "parallax.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

using namespace dagger;
using namespace platformer;

void ParallaxSystem::Run() 
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    auto camPosition = (Vector2)camera->position;

    Engine::Registry().view<Parallax, Sprite>().each([&](Parallax& parallax_, Sprite& sprite_)
        {
            auto delta = (parallax_.lastCameraPosition - camPosition) * parallax_.strength;
            sprite_.position.x += delta.x;
            sprite_.position.y += delta.y;
            parallax_.lastCameraPosition = camPosition;
        });
}
