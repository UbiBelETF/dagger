#include "racing_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/racing/racing_game_logic.h"
#include "gameplay/racing/racing_player_car.h"
#include "gameplay/racing/racing_car.h"

using namespace dagger;
using namespace racing_game;

void racing_game::SetupSystems(Engine &engine_)
{
    engine_.AddSystem<RacingPlayerInputSystem>();
    engine_.AddSystem<RacingCarSystem>();
    engine_.AddSystem<RacingCollisionsLogicSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
}

void racing_game::SetupWorld(Engine &engine_)
{
    ShaderSystem::Use("standard");

    Camera camera;
    camera.mode = ECameraMode::FixedResolution;
    camera.size = { 800, 600 };
    camera.zoom = 1.f;
    Engine::Dispatcher().trigger<Camera>(camera);

    auto& reg = engine_.Registry();

    constexpr Vector2 scale(1, 1);

    constexpr int Heigh = 30;
    constexpr int Width = 21;
    constexpr float TileSize = 0.15f;

    {
        auto entity = reg.create();
        auto& fieldSettings = reg.emplace<RacingGameFieldSettings>(entity);
        fieldSettings.fieldWidth = Width;
        fieldSettings.fieldHeight = Heigh;
        fieldSettings.fieldTileSize = TileSize;

        Engine::PutDefaultResource<RacingGameFieldSettings>(&fieldSettings);
    }

    float zPos = -1.f;

    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.scale = scale * TileSize;

            sprite.color = { 0.4f, 0.4f, 0.4f, 1 };

            if ((j == Width / 2) || (Width % 2 == 0 && (j == Width / 2 - 1)))
            {
                sprite.color = { 1,1,1,1 };
            }

            if (j == 0 || j == Width - 1)
            {
                sprite.color = { 0,0,0,1 };
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j - static_cast<float>(Width) / 2.f) * TileSize;
            transform.position.y = (0.5f + i - static_cast<float>(Heigh) / 2.f) * TileSize;
            transform.position.z = zPos;
        }
    }

    zPos += 0.5f;

    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        sprite.scale = { 2 * TileSize, 2 * TileSize };
        AssignSpriteTexture(sprite, "Racing:police-car-bmw-z4");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -TileSize * 4, -TileSize * 4, zPos };

        auto& racingPlayer = reg.emplace<RacingPlayerCar>(entity);
        racingPlayer.horzSpeed = TileSize * 6;

        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = { 2 * TileSize, 2 * TileSize };
    }

    // collisions for road bounds

    // other cars
    int amountOfCars = rand() % 3 + 3;
    for (int i = 0; i < amountOfCars; i++)
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        sprite.scale = { 2 * TileSize, -2 * TileSize };
        AssignSpriteTexture(sprite, "Racing:police-car-bmw-z4");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { TileSize * (3 * (i+1) - Width/2), TileSize * (-i * 2 + Heigh/2), zPos };

        auto& racingCar = reg.emplace<RacingCar>(entity);
        racingCar.speed = TileSize * (rand() % 5 + 3);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = { 2 * TileSize, 2 * TileSize };
    }
}
