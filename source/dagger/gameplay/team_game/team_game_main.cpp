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
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
    
    team_game::SetupWorld(engine_);
}

namespace jovica
{
    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
        int type = rand() % 10;
        switch (type)
        {
        default:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");
            break;
        case 0:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_2");
            break;
        case 1:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_3");
            break;
        case 2:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_4");
            break;
        case 3:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_5");
            break;
        case 4:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_6");
            break;
        case 5:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_7");
            break;
        case 6:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_8");
            break;
        case 7:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_9");
            break;
        case 8:
            AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_10");
            break;
        }
        return entity;
    }
}
namespace smiljana {
    
    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_)
    {
       
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16 , y_ * 16 , 30};
        AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");
        

        return entity;
    }
    Entity CreateWall(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
        AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_1");

        return entity;
    }
}
namespace kosta {
    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_) {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
        AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");

        return entity;
    }

    Entity CreateWall(Registry& reg_, UInt32 x_, UInt32 y_) {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
        AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_2");

        return entity;
    }

    Entity CreateDoor(Registry& reg_, UInt32 x_, UInt32 y_) {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 30 };
        AssignSprite(sprite, "spritesheets:among_them_tilemap:door_floor");

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

        Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/tilemap_test.map", &legend });

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
void SetupWorldSmiljana(Engine& engine_) {
  
        auto& reg = engine_.Registry();

        float zPos = 1.f;

        {
            TilemapLegend legend;
            legend['.'] = &smiljana::CreateFloor;
            legend['#'] = &smiljana::CreateWall;

            Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/my_first_map.map", &legend });

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
void SetupWorldKosta(Engine& enigne_) {
    auto& reg = enigne_.Registry();
    float zPos = 1.f;
    {
        TilemapLegend legend;
        legend['.'] = &kosta::CreateFloor;
        legend['#'] = &kosta::CreateWall;
       // legend['>'] = &kosta::CreateDoor;
        Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/kostaLevel.map", &legend });
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
   // SetupWorldJovica(engine_);
  //  SetupWorldSmiljana(engine_);
    SetupWorldKosta(engine_);
}
