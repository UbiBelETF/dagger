#include "camera_follow.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include <gameplay/team_game/team_game_main.h>
#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/camera_follow.h"
#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/game_manager.h"

using namespace dagger;
using namespace team_game;

void CameraFollowSystem::DistributeCameraWeight()
{

    auto view = Engine::Registry().view<Collider, TreasureChest, Transform>();
    auto& iterator = view.begin();
    while (iterator != view.end())
    {
        auto&& [treasureCollider, treasureChest, treasureTransform] = Engine::Registry().get<Collider, TreasureChest, Transform>(*iterator);

        auto view1 = Engine::Registry().view<Transform, CameraFollowFocus, PlayerCharacter>();
        auto& it1 = view1.begin();

        while (it1 != view1.end())
        {
            auto&& [player, transform, focus] = Engine::Registry().get<PlayerCharacter, Transform, CameraFollowFocus>(*it1);

            player.distanceToChest = CalculateDistance(treasureTransform, transform);
            focus.weight = 1;

            it1++;
        }

        it1 = view1.begin();

        if (it1 != view1.end())
        {
            Float32 minDistance = Engine::Registry().get<PlayerCharacter>(*it1).distanceToChest;
            entt::entity entity = *it1;

            while (it1 != view1.end())
            {
                auto& player = Engine::Registry().get<PlayerCharacter>(*it1);

                if (player.distanceToChest < minDistance)
                {
                    minDistance = player.distanceToChest;
                    entity = *it1;
                }
                it1++;
            }

            Engine::Registry().get<CameraFollowFocus>(entity).weight = 2;
        }
        iterator++;
    }
}

void CameraFollowSystem::Run()
{
    DistributeCameraWeight();

    AdjustCameraZoom();

    auto* camera = Engine::GetDefaultResource<Camera>();
    auto camPosition = (Vector2)camera->position;

    Vector2 center{};
    UInt32 count{ 0 };

    Engine::Registry().view<CameraFollowFocus, Sprite>().each([&](const CameraFollowFocus& focus_, const Sprite& sprite_)
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
        camera->position = Vector3{ glm::mix((Vector2)camera->position, center, 0.02f), 0.0f };
    }
    //camera->position.y += 100.f;
}

void CameraFollowSystem::AdjustCameraZoom()
{
    auto view = Engine::Registry().view<PlayerCharacter, Transform>();
    auto iterator = view.begin();

    if (iterator != view.end()) {

        Float32 maxDistance = 0.f;

        while (iterator != view.end())
        {
            auto& transform = Engine::Registry().get<Transform>(*iterator);

            auto iterator2 = iterator;
            iterator2++;

            while (iterator2 != view.end())
            {

                auto& tr = Engine::Registry().get<Transform>(*iterator2);

                Float32 distance = CalculateDistance(transform, tr);

                if (distance > maxDistance)
                {
                    maxDistance = distance;
                }
                iterator2++;
            }
            iterator++;
        }

        auto* camera = Engine::GetDefaultResource<Camera>();

        if (maxDistance >= 1100)
        {
            camera->zoom = 0.3;
        }
        else if (maxDistance <= 300)
        {
            camera->zoom = 0.9;
        }
        else
        {
            //the 1 is the max zoom and the division transforms the interval of
            //the two camera zoom locking distances(in this case (500, 650))
            //to an interval of max zoom - min zoom(in this case 0.25)
            camera->zoom = 0.9 - (maxDistance - 300) / (1333);
        }
    }
}

Float32 CameraFollowSystem::CalculateDistance(Transform& transformOne_, Transform& transformTwo_)
{
    return std::sqrt(std::pow((transformOne_.position.x - transformTwo_.position.x), 2) + std::pow((transformOne_.position.y - transformTwo_.position.y), 2));
}


