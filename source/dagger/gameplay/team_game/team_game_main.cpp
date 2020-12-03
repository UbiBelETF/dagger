#include "team_game_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

void team_game::SetupWorld(Engine &engine_)
{
    Vector2 scale(1, 1);

    auto& reg = engine_.Registry();

    // field
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 600;

    constexpr int height = 40;
    constexpr int width = 30;
    constexpr float tileSize = 30.f;
    
    float zPos = 1.f;

    // 40x30 je teren (nemamo plocice iscrtane vise pa je nebitno)
    for (int i = -25; i < 26; i++)
    {
        for (int j = -20; j < 20; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);

            //AssignSprite(sprite, fmt::format("spritesheets:dungeon:wall_top_left", 1 + (rand() % 8)));
            AssignSprite(sprite, "tile_new");
            sprite.size = scale * tileSize;
            sprite.position = { i * 16, j * 16, 99 };

            if (j == 17) 
            {
                AssignSprite(sprite, fmt::format("spritesheets:tileset:wall_upper_mid", 1 + (rand() % 8)));
               
            }
            if(j == -18 || j == -19 ) 
            {
                AssignSprite(sprite, fmt::format("spritesheets:tileset:wall_down_mid", 1 + (rand() % 8)));
            }

            if(i == -24)
                AssignSprite(sprite, fmt::format("spritesheets:tileset:wall_upper_mid", 1 + (rand() % 8)));
            if(i == 25 )
                AssignSprite(sprite, fmt::format("spritesheets:tileset:wall_upper_mid", 1 + (rand() % 8)));

            if ((j == -1) && (i != 2 && i != 3 && i != -23 && i != 16))
            {
                AssignSprite(sprite, fmt::format("spritesheets:tileset:wall_upper_mid", 1 + (rand() % 8)));
            }

            if (j == 13 && i == -10)
            {
                AssignSprite(sprite, fmt::format("spritesheets:dungeon:doors_leaf_open"));
                //AssignSprite(sprite, "parti_team_game:door_front");
            }

                /*auto& transform = reg.emplace<Transform>(entity);
                transform.position.x = (0.5f + j + j * Space - static_cast<float>(width * (1 + Space)) / 2.f) * tileSize;
                transform.position.y = (0.5f + i + i * Space - static_cast<float>(height * (1 + Space)) / 2.f) * tileSize;
                transform.position.z = zPos;*/
        }
    }
    /*
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "logos:dagger");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 500 / ratio, 500  };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }*/
}
