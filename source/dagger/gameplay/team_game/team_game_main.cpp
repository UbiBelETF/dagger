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
#include "gameplay/team_game/enemy.h"
#include "gameplay/team_game/door_interaction.h"



#include "gameplay/team_game/camera.h"
#include "gameplay/team_game/tilemap.h"
#include "gameplay/team_game/level_generator.h"
#include "gameplay/team_game/movement.h"
#include "gameplay/team_game/physics.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<EnemyControllerSystem>();
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<CameraSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<PhysicsSystem>();
    engine_.AddSystem<MovementSystem>();
    engine_.AddSystem<DoorSystem>();
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

void SetupWorldJovica(Engine& engine_, Registry& reg_)
{
    // TILEMAP
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
    auto player = reg_.create();

    auto& playerState = ATTACH_TO_FSM(CharacterFSM, player);
    playerState.currentState = ECharacterState::Idle;

    auto& playerSprite = reg_.emplace<Sprite>(player);
    AssignSprite(playerSprite, "spritesheets:among_them_spritesheet:knight_idle_anim:1");
    playerSprite.scale = { 1, 1 };

    auto& playerAnimator = reg_.emplace<Animator>(player);
    AnimatorPlay(playerAnimator, "among_them_animations:knight_idle");

    auto& playerTransform = reg_.emplace<Transform>(player);
    playerTransform.position = { 50, -50, 1 };

    auto& playerInput = reg_.get_or_emplace<InputReceiver>(player);
    playerInput.contexts.push_back("AmongThemInput");

    reg_.emplace<CharacterController>(player);

    reg_.emplace<MovableBody>(player);
}

void SetupWorldSmiljana(Engine& engine_, Registry& reg_) {
    



    {
        TilemapLegend legend;
        legend['.'] = &level_generator::smiljana::CreateFloor;
        legend['#'] = &level_generator::smiljana::CreateWall;


        Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/my_first_map.map", &legend });

        // PLAYER
        auto player = reg_.create();

        auto& playerState = ATTACH_TO_FSM(CharacterFSM, player);
        playerState.currentState = ECharacterState::Idle;


    

        auto& playerSprite = reg_.emplace<Sprite>(player);
        AssignSprite(playerSprite, "spritesheets:among_them_spritesheet:knight_idle_anim:1");
        playerSprite.scale = { 1, 1 };

        auto& playerAnimator = reg_.emplace<Animator>(player);
        AnimatorPlay(playerAnimator, "among_them_animations:knight_idle");

        auto& playerTransform = reg_.emplace<Transform>(player);
        playerTransform.position = { 0, 0, 1 };

        auto& playerInput = reg_.get_or_emplace<InputReceiver>(player);
        playerInput.contexts.push_back("AmongThemInput");

        reg_.emplace<CharacterController>(player);

        auto& movable = reg_.emplace<MovableBody>(player);
        movable.size = playerSprite.size;

        //DOOR

        auto door = reg_.create();

        auto& doorSprite = reg_.emplace<Sprite>(door);
        AssignSprite(doorSprite, "spritesheets:among_them_spritesheet:door_open_anim:1");
        doorSprite.scale = { 1, 1 };

        auto& doorAnimator = reg_.emplace<Animator>(door);
        
        auto& doorTransform = reg_.emplace<Transform>(door);
        doorTransform.position = { 45, 20, 1 };

        auto& doorCollision = reg_.emplace<SimpleCollision>(door);
        doorCollision.size = doorSprite.size;
        
       reg_.emplace<Door>(door);
        
    }
}
void team_game::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    float zPos = 1.f;

    // STATIC BODIES MAP
    auto mapEnt = reg.create();
	  auto& map = reg.emplace<StaticBodyMap>(mapEnt);
	  Engine::PutDefaultResource<StaticBodyMap>(&map);
	
    // You can add your own WorldSetup functions when testing, call them here and comment out mine
    //SetupWorldJovica(engine_, reg);
      SetupWorldSmiljana(engine_, reg);

}


