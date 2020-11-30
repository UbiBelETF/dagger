#include "team_game_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/animations.h"

#include "gameplay/common/simple_collisions.h"

#include "gameplay/team_game/character_controller.h"
#include "gameplay/team_game/tilemap.h"
#include "gameplay/team_game/camera.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<CameraSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 3;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

namespace jovica
{
    Entity CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
       
        int type = 1 + rand() % 10;
        AssignSprite(sprite, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));
        
        return entity;
    }

    Entity CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };

        int type = 1 + rand() % 4;
        AssignSprite(sprite, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));

        Entity top = reg_.create();
        auto& topSprite = reg_.emplace<Sprite>(top);
        topSprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

        AssignSprite(topSprite, "spritesheets:among_them_tilemap:wall_top");

        return entity;
    }

    Entity CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };

        int type = 1 + rand() % 4;
        AssignSprite(sprite, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));

        Entity top = reg_.create();
        auto& topSprite = reg_.emplace<Sprite>(top);
        topSprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

        AssignSprite(topSprite, "spritesheets:among_them_tilemap:wall_bottom");

        return entity;
    }
}

void SetupWorldJovica(Engine& engine_)
{
    auto& reg = engine_.Registry();

    float zPos = 1.f;

    {
        TilemapLegend legend;
        legend['.'] = &jovica::CreateFloor;
        legend['_'] = &jovica::CreateTopWall;
        legend['-'] = &jovica::CreateBottomWall;

        Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/tilemap_test_jovica.map", &legend });

        // PLAYER
        auto player = reg.create();

        auto& playerSprite = reg.emplace<Sprite>(player);
        AssignSprite(playerSprite, "spritesheets:among_them_spritesheet:knight_idle_anim:1");

        auto& playerAnimator = reg.emplace<Animator>(player);
        AnimatorPlay(playerAnimator, "among_them_animations:knight_idle");

        auto& playerTransform = reg.emplace<Transform>(player);
        playerTransform.position = { 0, 0, zPos };

        auto& playerInput = reg.get_or_emplace<InputReceiver>(player);
        playerInput.contexts.push_back("AmongThemInput");

        auto& playerController = reg.emplace<CharacterController>(player);
    }
}

void team_game::SetupWorld(Engine &engine_)
{
    // You can add your own WorldSetup functions when testing, call them here and comment out mine
    SetupWorldJovica(engine_);
}
