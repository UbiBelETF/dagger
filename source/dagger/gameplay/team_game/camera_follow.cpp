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

            player.distanceToChest = CalculateDistanceToTreasure(treasureTransform, transform);
            focus.weight = 1;

            it1++;
        }

        it1 = view1.begin();

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

        iterator++;
    }
}

void CameraFollowSystem::Run()
{
    DistributeCameraWeight();

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

Float32 CameraFollowSystem::CalculateDistanceToTreasure(Transform& treasure_, Transform& player_)
{
    return std::sqrt(std::pow((treasure_.position.x - player_.position.x), 2) + std::pow((treasure_.position.y - player_.position.y), 2));
}


