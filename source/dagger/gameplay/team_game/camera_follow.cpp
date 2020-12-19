#include "camera_follow.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

using namespace dagger;
using namespace team_game;

void CameraFollowSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();

    Engine::Registry().view<CameraFollowFocus, Sprite>().each([&](const CameraFollowFocus& focus_, const Sprite& sprite_)
        {
            camera->position = sprite_.position;
        });
}
