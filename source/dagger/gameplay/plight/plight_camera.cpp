#include "plight_camera.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

using namespace dagger;
using namespace plight;

void CameraCenterSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    auto camPosition = (Vector2)camera->position;

    auto camEnt = Engine::Registry().view<CameraParams>().front();
    auto camParams = Engine::Registry().get<CameraParams>(camEnt);
    Float32 camZoom = camParams.camZoom;
    Vector2 camXY = camParams.camXY;
    camXY /= 2;

    Vector2 center{};
    UInt32 count{ 0 };
    Vector2 distance{ 0, 0 };
    Vector2 s{ 1, 1 };

    Engine::Registry().view<CameraCenter, Sprite>().each([&](const CameraCenter& focus_, const Sprite& sprite_)
        {
            distance += (Vector2)sprite_.position * s;
            s = { -1, -1 };
        });

    distance = abs(distance);

    if ((distance.x > (camXY.x / camera->zoom) || distance.x < -(camXY.x / camera->zoom))
        || (distance.y > (camXY.y / camera->zoom) || distance.y < -(camXY.y / camera->zoom)))
    {
        camera->zoom -= 0.001f;
    }
    if ((distance.x < ((camXY.x / camera->zoom)) && (distance.x > camXY.x / camZoom))
        || (distance.y < ((camXY.y / camera->zoom)) && (distance.y > camXY.y / camZoom)))
    {
        camera->zoom += 0.001f;
    }

    Engine::Registry().view<CameraCenter, Sprite>().each([&](const CameraCenter& focus_, const Sprite& sprite_)
        {
            for (UInt32 i = 0; i < focus_.weight; i++)
            {
                center += (Vector2)sprite_.position;
            }
            count += focus_.weight;
        });

    if (count > 0)
    {
        center /= count;
        camera->position = Vector3{ glm::mix((Vector2)camera->position, center, 0.5f), 0.0f };
    }
}
