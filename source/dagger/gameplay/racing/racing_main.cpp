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
#include "gameplay/racing/racing_big_laser.h"

using namespace dagger;
using namespace racing_game;

void RacingGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<RacingPlayerInputSystem>();
    engine_.AddSystem<RacingCarSystem>();
    engine_.AddSystem<RacingCollisionsLogicSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<BigLaserSystem>();
}

void RacingGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 600, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    racing_game::SetupWorld(engine_);
}

void racing_game::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    constexpr Vector2 scale(1, 1);

    constexpr int Heigh = 30;
    constexpr int Width = 21;
    constexpr float TileSize = 20.f;

    {
        auto entity = reg.create();
        auto& fieldSettings = reg.emplace<RacingGameFieldSettings>(entity);
        fieldSettings.fieldWidth = Width;
        fieldSettings.fieldHeight = Heigh;
        fieldSettings.fieldTileSize = TileSize;

        Engine::PutDefaultResource<RacingGameFieldSettings>(&fieldSettings);
    }

    float zPos = 10.f;

    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.size = scale * TileSize;

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

    zPos = 5.f;

    // player
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "Racing:police-car-bmw-z4-bfl");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 2 * TileSize * ratio };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -TileSize * 4, -TileSize * 4, zPos };

        auto& racingPlayer = reg.emplace<RacingPlayerCar>(entity);
        racingPlayer.horzSpeed = TileSize * 6;

        reg.emplace<ControllerMapping>(entity);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }
    
    zPos = 4.f;

    //players laser entity
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "Racing:laser-long-off");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 5 * TileSize * ratio };
        sprite.position.z = zPos;
        
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { -TileSize * 4, TileSize * 4, zPos };

        reg.emplace<ControllerMapping>(entity);

        auto& laserSystem = reg.emplace<BigLaserGun>(entity);
        laserSystem.laserSpeed = TileSize * 6;

        
    }

    zPos = 5.f;

    // collisions for road bounds

    // other cars
    int amountOfCars = rand() % 3 + 3;
    for (int i = 0; i < amountOfCars; i++)
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "Racing:police-car-bmw-z4"); 
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 2 * TileSize, 2 * TileSize * ratio };
        sprite.scale.y = -1;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { TileSize * (3 * (i+1) - Width/2), TileSize * (-i * 2 + Heigh/2), zPos };

        auto& racingCar = reg.emplace<RacingCar>(entity);
        racingCar.speed = TileSize * (rand() % 5 + 3);

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }
}
