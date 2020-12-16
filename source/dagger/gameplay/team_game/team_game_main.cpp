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
#include "gameplay/team_game/camera.h"

#include "gameplay/team_game/tilemap.h"

#include "gameplay/team_game/camera.h"
#include "gameplay/team_game/movement.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<CameraSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<MovementSystem>();
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

namespace smiljana {
    
    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_)
    {
       
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16 - 125.0f, y_ * 16 - 50.0f, 30 };
        AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");
        

        return entity;
    }
    Entity CreateWall(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16 - 125.0f, y_ * 16 - 50.0f, 30 };
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
        auto& collision = reg_.emplace<SimpleCollision>(entity);
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

            auto& playerState = ATTACH_TO_FSM(CharacterFSM, player);
            playerState.currentState = ECharacterState::Idle;

            auto& playerSprite = reg.emplace<Sprite>(player);
            AssignSprite(playerSprite, "spritesheets:among_them_spritesheet:knight_idle_anim:1");
            playerSprite.scale = { 1, 1 };

            auto& playerAnimator = reg.emplace<Animator>(player);
            AnimatorPlay(playerAnimator, "among_them_animations:knight_idle");

            auto& playerTransform = reg.emplace<Transform>(player);
            playerTransform.position = { 0, 0, zPos };

            auto& playerInput = reg.get_or_emplace<InputReceiver>(player);
            playerInput.contexts.push_back("AmongThemInput");

        reg.emplace<CharacterController>(player);

        reg.emplace<MovableBody>(player);
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
        {
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

            auto& collision = reg.emplace<SimpleCollision>(player);
        }
    }
}
void team_game::SetupWorld(Engine &engine_)
{
    // You can add your own WorldSetup functions when testing, call them here and comment out mine
   // SetupWorldJovica(engine_);
    //SetupWorldKosta(engine_);
   
    SetupWorldSmiljana(engine_);
}
