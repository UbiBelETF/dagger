#include "plight_camera.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "gameplay/plight/plight_game_logic.h"

using namespace dagger;
using namespace plight;

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

void CameraCenterSystem::Run()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    auto camPosition = (Vector2)camera->position;

    auto camEnt = Engine::Registry().view<CameraParams>().front();
    auto camParams = Engine::Registry().get<CameraParams>(camEnt);
    Float32 newZoom = camera->zoom;
    Vector2 camXY = camParams.camXY;
    float distanceFact = 1.6;

    float maxZoom = 1.50;
    float minZoom = 1;

    Vector2 center{};
    UInt32 count{ 0 };
    Vector2 distance{ 0, 0 };
    Vector2 s{ 1, 1 };

    auto gameInfoEnt = Engine::Registry().view<PlightGameInfo>().front();
    PlightGameInfo gameInfo = Engine::Registry().get<PlightGameInfo>(gameInfoEnt);
    if (gameInfo.displayingMessageEndGame || gameInfo.displayingMessage) {
        camera->position = { 0.0f,0.0f,0.0f };
        camera->zoom = camParams.camZoom;
        return;
    }

    Engine::Registry().view<CameraCenter, Sprite>().each([&](const CameraCenter& focus_, const Sprite& sprite_)
        {
            distance += (Vector2)sprite_.position * s;
            s = { -1, -1 };
        });

    distance = abs(distance);
    camXY = camParams.camXY / distanceFact;

    if ((distance.x > abs(camXY.x / camera->zoom)) || (distance.y > abs(camXY.y / camera->zoom)))
    {
        newZoom = min(abs(camXY.x / distance.x), abs(camXY.y / distance.y));
    }

    if ((distance.x < abs(camXY.x / camera->zoom)) && (distance.y < abs(camXY.y / camera->zoom)))
    {
        newZoom = min(abs(camXY.x / distance.x), abs(camXY.y / distance.y));
    }
    
    if (newZoom > minZoom && newZoom < maxZoom) camera->zoom = newZoom;

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
        camXY = camParams.camXY;
        center /= count;
        Vector3 newPos = Vector3{ glm::mix((Vector2)camera->position, center, 0.5f), 0.0f };

        int startX = newPos.x + (camXY.x - camXY.x / camera->zoom) / 2;
        int endX = startX + camXY.x / camera->zoom;
        int startY = newPos.y + (camXY.y - camXY.y / camera->zoom) / 2;
        int endY = startY + camXY.y / camera->zoom;

        if (startX < 0 || endX > camXY.x || startY < 0 || endY > camXY.y) {
            if (startX < 0) newPos.x = -(camXY.x - camXY.x / camera->zoom) / 2;
            else if (endX > camXY.x)  newPos.x = (1 - 1 / camera->zoom)* camXY.x / 2;

            if (startY < 0) newPos.y = -(camXY.y - camXY.y / camera->zoom) / 2;
            else if (endY > camXY.y)  newPos.y = (1 - 1 / camera->zoom) * camXY.y / 2;

            if (abs(camera->position.x - newPos.x) > 1 || abs(camera->position.y - newPos.y) > 1 )
                camera->position = newPos;
        } else 
            camera->position = newPos;
    }
}
