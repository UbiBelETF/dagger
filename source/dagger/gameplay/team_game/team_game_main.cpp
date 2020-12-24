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
#include "gameplay/team_game/key.h"


#include "gameplay/team_game/camera.h"
#include "gameplay/team_game/tilemap.h"
#include "gameplay/team_game/level_generator.h"
#include "gameplay/team_game/movement.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/follow.h""
#include "gameplay/team_game/remote_animation.h"
#include <gameplay\team_game\detection.h>
#include <gameplay\team_game\chasing.h>

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
    engine_.AddSystem<KeySystem>();
    engine_.AddSystem<FollowSystem>();
    engine_.AddSystem<RemoteAnimationSystem>();
    engine_.AddSystem<DetectionSystem>();
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

    auto& controller = reg_.emplace<CharacterController>(player);

    reg_.emplace<MovableBody>(player);

    // POOF
    auto poofEntity = reg_.create();
    
    reg_.emplace<Transform>(poofEntity);

    auto& poofSprite = reg_.emplace<Sprite>(poofEntity);
    AssignSprite(poofSprite, "spritesheets:among_them_spritesheet:poof_anim:5");
    poofSprite.scale = { 0.5, 0.5 };

    auto& poofFollow = reg_.emplace<Follow>(poofEntity);
    poofFollow.target = player;
    poofFollow.offset.z = -1;

    auto& poofAnimator = reg_.emplace<Animator>(poofEntity);
    poofAnimator.isLooping = false;

    auto& exec = reg_.emplace<AnimationExecutor>(poofEntity);
    exec.source = &controller.animationTrigger;
    exec.animationName = "among_them_animations:poof";
    exec.startingSpriteName = "spritesheets:among_them_spritesheet:poof_anim:1";
}

void SetupWorldSmiljana(Engine& engine_, Registry& reg_) {
    

    Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/my_first_map.map", &legend });
  
 // Wall
 /*   auto wall = reg_.create();


    
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

    auto& wallSprite2 = reg_.emplace<Sprite>(wall2);
    AssignSprite(wallSprite2, "EmptyWhitePixel");
    wallSprite2.color = { 0.0f, 0.0f, 0.0f, 1.0f };
    wallSprite2.size = { 30, 30 };
    auto& st2 = reg_.emplace<StaticBody>(wall2);
    st2.size = wallSprite2.size;*/
      
    // PLAYER
    auto player = reg_.create();

        auto& playerAnimator = reg_.emplace<Animator>(player);
        AnimatorPlay(playerAnimator, "among_them_animations:knight_idle");

        auto& playerTransform = reg_.emplace<Transform>(player);
        playerTransform.position = { 0, 0, 1 };

        auto& playerInput = reg_.get_or_emplace<InputReceiver>(player);
        playerInput.contexts.push_back("AmongThemInput");

        auto& playerCollision = reg_.emplace<SimpleCollision>(player);
        playerCollision.size = playerSprite.size;

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
        doorTransform.position = { 40, 20, 1 };


    auto& detection = reg_.emplace<Detection>(player);
    detection.SetSize({ 2,2 });

        auto& doorCollision = reg_.emplace<SimpleCollision>(door);
        doorCollision.size = doorSprite.size;
        
        SInt32 x = ((SInt32)doorTransform.position.x + 8) / 16;
        SInt32 y = ((SInt32)doorTransform.position.y - 8) / 16;

        auto& collider = reg_.emplace<StaticBody>(door);
        collider.size = doorSprite.size;
        Engine::GetDefaultResource<StaticBodyMap>()->put(x,y, door);

        reg_.emplace<Door>(door);

       

       //KEY

       auto key = reg_.create();

       auto& keySprite = reg_.emplace<Sprite>(key);
       AssignSprite(keySprite, "spritesheets:among_them_tilemap:key");
       keySprite.scale = { 1, 1 };

       auto& keyTransform = reg_.emplace<Transform>(key);
       keyTransform.position = { 45, -40, 1 };

       auto& keyCollision = reg_.emplace<SimpleCollision>(key);
       keyCollision.size = keySprite.size;

       reg_.emplace<Key>(key);
       
    


    //ENEMY 
    auto enemy = reg_.create();

    auto& enemyState = ATTACH_TO_FSM(EnemyFSM, enemy);
    enemyState.currentState = EEnemyState::Patrolling;

    auto& enemySprite = reg_.emplace<Sprite>(enemy);
    AssignSprite(enemySprite, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite.scale = { 1, 1 };

    auto& enemyAnimator = reg_.emplace<Animator>(enemy);
    AnimatorPlay(enemyAnimator, "among_them_animations:goblin_idle");

    auto& enemyTransform = reg_.emplace<Transform>(enemy);
    enemyTransform.position = { 0, 25, 1 };

    auto& enemyInput = reg_.emplace<InputEnemiesFile>(enemy);
    enemyInput.pathname = "path.txt";
    enemyInput.currentshape = "goblin";

    auto& en = reg_.emplace<EnemyDescription>(enemy);
    en.shape = ECharacterShape::Goblin;

    reg_.emplace<MovableBody>(enemy);

    auto& enDetection1 = reg_.emplace<Detection>(enemy);
    enDetection1.SetSize(en.detectionArea);


    //ENEMY NO.2

    //ENEMY 
    auto enemy2 = reg_.create();

    auto& enemy2State = ATTACH_TO_FSM(EnemyFSM, enemy2);
    enemy2State.currentState = EEnemyState::Patrolling;

    auto& enemy2Sprite = reg_.emplace<Sprite>(enemy2);
    AssignSprite(enemy2Sprite, "spritesheets:among_them_spritesheet:bat_anim:1");
    enemy2Sprite.scale = { 1, 1 };

    auto& enemy2Animator = reg_.emplace<Animator>(enemy2);
    AnimatorPlay(enemy2Animator, "among_them_animations:bat");

    auto& enemy2Transform = reg_.emplace<Transform>(enemy2);
    enemy2Transform.position = { 0, 60, 1 };

    auto& enemy2Input = reg_.emplace<InputEnemiesFile>(enemy2);
    enemy2Input.pathname = "pathbat.txt";
    enemy2Input.currentshape = "bat";

    auto& en2 = reg_.emplace<EnemyDescription>(enemy2);
    en2.shape = ECharacterShape::Bat;

    reg_.emplace<MovableBody>(enemy2);

    auto& det2 = reg_.emplace<Detection>(enemy2);
    det2.SetSize(en2.detectionArea);

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

