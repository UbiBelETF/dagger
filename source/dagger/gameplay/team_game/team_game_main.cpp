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
#include "gameplay/team_game/level_generator.h"
#include "gameplay/team_game/movement.h"
#include "gameplay/team_game/physics.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<CameraSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<PhysicsSystem>();
    engine_.AddSystem<MovementSystem>();
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

void SetupWorldJovica(Engine& engine_)
{
    auto& reg = engine_.Registry();

    float zPos = 1.f;

    {
        TilemapLegend legend;
        legend[' '] = &level_generator::jovica::Nothing;
        legend['.'] = &level_generator::jovica::CreateFloor;
        legend['_'] = &level_generator::jovica::CreateTopWall;
        legend['-'] = &level_generator::jovica::CreateBottomWall;
        legend['/'] = &level_generator::jovica::CreateLeftWall;
        legend['\\'] = &level_generator::jovica::CreateRightWall;
        legend['T'] = &level_generator::jovica::CreateTopLeftWall;
        legend['Y'] = &level_generator::jovica::CreateTopRightWall;
        legend['L'] = &level_generator::jovica::CreateBottomLeftWall;
        legend['J'] = &level_generator::jovica::CreateBottomRightWall;
        legend['q'] = &level_generator::jovica::CreateBottomLeftConcWall;
        legend['p'] = &level_generator::jovica::CreateBottomRightConcWall;
        legend['d'] = &level_generator::jovica::CreateTopLeftConcWall;
        legend['b'] = &level_generator::jovica::CreateTopRightConcWall;
        legend['l'] = &level_generator::jovica::CreateBottomLeftConcWallS;
        legend['j'] = &level_generator::jovica::CreateBottomRightConcWallS;
        legend['t'] = &level_generator::jovica::CreateTopLeftConcWallS;
        legend['y'] = &level_generator::jovica::CreateTopRightConcWallS;

        Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/tilemap_test_jovica.map", &legend });

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
        playerTransform.position = { 50, -50, zPos };

        auto& playerInput = reg.get_or_emplace<InputReceiver>(player);
        playerInput.contexts.push_back("AmongThemInput");

        reg.emplace<CharacterController>(player);

        reg.emplace<MovableBody>(player);
    }
}

void SetupWorldSmiljana(Engine& engine_) {
  
        auto& reg = engine_.Registry();

        float zPos = 1.f;

        {
            TilemapLegend legend;
            legend['.'] = &level_generator::smiljana::CreateFloor;
            legend['#'] = &level_generator::smiljana::CreateWall;

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

        auto& movable = reg.emplace<MovableBody>(player);
        movable.size = playerSprite.size;

        // Wall
        auto wall = reg.create();

        auto& wallTransform = reg.emplace<Transform>(wall);
        wallTransform.position = { 0, 0, 0 };

        auto& wallSprite = reg.emplace<Sprite>(wall);
        AssignSprite(wallSprite, "EmptyWhitePixel");
        wallSprite.color = { 0.0f, 0.0f, 0.0f, 1.0f };
        wallSprite.size = { 30, 30 };

        auto& st = reg.emplace<StaticBody>(wall);
        st.size = wallSprite.size;

        auto wall1 = reg.create();

        auto& wallTransform1 = reg.emplace<Transform>(wall1);
        wallTransform1.position = { 30, 0, 0 };

        auto& wallSprite1 = reg.emplace<Sprite>(wall1);
        AssignSprite(wallSprite1, "EmptyWhitePixel");
        wallSprite1.color = { 0.0f, 0.0f, 0.0f, 1.0f };
        wallSprite1.size = { 30, 30 };

        auto& st1 = reg.emplace<StaticBody>(wall1);
        st1.size = wallSprite1.size;

        auto wall2 = reg.create();

        auto& wallTransform2 = reg.emplace<Transform>(wall2);
        wallTransform2.position = { 0, 30, 0 };

        auto& wallSprite2 = reg.emplace<Sprite>(wall2);
        AssignSprite(wallSprite2, "EmptyWhitePixel");
        wallSprite2.color = { 0.0f, 0.0f, 0.0f, 1.0f };
        wallSprite2.size = { 30, 30 };

        auto& st2 = reg.emplace<StaticBody>(wall2);
        st2.size = wallSprite2.size;
    }
}
void team_game::SetupWorld(Engine &engine_)
{
    // You can add your own WorldSetup functions when testing, call them here and comment out mine
    SetupWorldJovica(engine_);
    //SetupWorldKosta(engine_);
}
