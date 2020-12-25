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
#include "gameplay/team_game/game_controller.h"
#include "gameplay/team_game/follow.h"
#include "gameplay/team_game/remote_animation.h"
#include "gameplay/team_game/detection.h"
#include "gameplay//team_game/vision_cone.h"

using namespace dagger;
using namespace team_game;
void TeamGame::GameplaySystemsSetup(Engine& engine_)
{
    engine_.AddSystem<EnemyControllerSystem>();
    engine_.AddSystem<CharacterControllerSystem>();
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<DoorSystem>();
    engine_.AddSystem<KeySystem>();
    engine_.AddSystem<RemoteAnimationSystem>();
    engine_.AddSystem<DetectionSystem>();
    engine_.AddSystem<VisionConeSystem>();
    engine_.AddSystem<PhysicsSystem>();
    engine_.AddSystem<MovementSystem>();
    engine_.AddSystem<FollowSystem>();
    engine_.AddSystem<CameraSystem>();
    engine_.AddSystem<GameControllerSystem>();
}

void TeamGame::WorldSetup(Engine& engine_)
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
void SetupWorld_(Engine& engine_, Registry& reg_)
{
    // TILEMAP
    TilemapLegend legend;
	
    legend[' '] = &level_generator::jovica::Nothing;
    legend['.'] = &level_generator::jovica::CreateFloor;
    legend['_'] = &level_generator::jovica::CreateTopWall;
    legend['-'] = &level_generator::jovica::CreateBottomWall;
    legend['/'] = &level_generator::jovica::CreateLeftWall;
    legend['\\'] = &level_generator::jovica::CreateRightWall;
    legend['E'] = &level_generator::jovica::CreateTopLeftWall;
    legend['Y'] = &level_generator::jovica::CreateTopRightWall;
    legend['L'] = &level_generator::jovica::CreateBottomLeftWall;
    legend['J'] = &level_generator::jovica::CreateBottomRightWall;
    legend['q'] = &level_generator::jovica::CreateBottomLeftConcWall;
    legend['p'] = &level_generator::jovica::CreateBottomRightConcWall;
    legend['d'] = &level_generator::jovica::CreateTopLeftConcWall;
    legend['!'] = &level_generator::jovica::CreateTopRightConcWall;
    legend['l'] = &level_generator::jovica::CreateBottomLeftConcWallS;
    legend['j'] = &level_generator::jovica::CreateBottomRightConcWallS;
    legend['e'] = &level_generator::jovica::CreateTopLeftConcWallS;
    legend['y'] = &level_generator::jovica::CreateTopRightConcWallS;
    legend['@'] = &level_generator::jovica::CreatePlayer;
    legend['d'] = &level_generator::jovica::CreateDoor;
    legend['g'] = &level_generator::jovica::CreateIdleGoblin; 
    legend['k'] = &level_generator::jovica::CreateKey;
    legend['T'] = &level_generator::jovica::CreateBigTable;
    legend['b'] = &level_generator::jovica::CreateBookShelfOnTopWall;
    legend['P'] = &level_generator::jovica::CreatePrisonerOnTopWall;
    legend['c'] = &level_generator::jovica::CreateChest;
    legend['i'] = &level_generator::jovica::CreateGreenBanner;
    legend['r'] = &level_generator::jovica::CreateRedBanner;
    legend['f'] = &level_generator::jovica::CreateTorch;
    legend['o'] = &level_generator::jovica::CreateBarrel;
    legend['s'] = &level_generator::jovica::CreateSmallTable;
    Engine::Dispatcher().trigger <TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/tilemap_test_jovica.map", &legend });

    //slime1
    auto slime1 = reg_.create();
    auto& enemyState = ATTACH_TO_FSM(EnemyFSM, slime1);
    enemyState.currentState = EEnemyState::Patrolling;

    auto& enemySprite = reg_.emplace<Sprite>(slime1);
    AssignSprite(enemySprite, "spritesheets:among_them_spritesheet:slime_idle_anim:1");
    enemySprite.scale = { 1, 1 };

    auto& enemyAnimator = reg_.emplace<Animator>(slime1);
    AnimatorPlay(enemyAnimator, "among_them_animations:slime_idle");

    auto& enemyTransform = reg_.emplace<Transform>(slime1);
    enemyTransform.position = { 18 * 16, (-19) * 16, 1 };

    auto& enemyInput = reg_.emplace<InputEnemiesFile>(slime1);
    enemyInput.pathname = "slime1.txt";
    enemyInput.currentshape = "slime";

    auto& en = reg_.emplace<EnemyDescription>(slime1);
    en.shape = ECharacterShape::Slime;

    en.enemyIdle = false;

    auto& enemyCollision = reg_.emplace<SimpleCollision>(slime1);
    enemyCollision.size = enemySprite.size;

    auto& det = reg_.emplace<Detection>(slime1);
    det.SetSize(en.detectionArea);

    reg_.emplace<MovableBody>(slime1);

    auto visionCone = reg_.create();

    reg_.emplace<Transform>(visionCone);

    auto& vcSprite = reg_.emplace<Sprite>(visionCone);
    AssignSprite(vcSprite, "AmongThem:circle");
    vcSprite.size = det.size;

    auto& vcFollow = reg_.emplace<Follow>(visionCone);
    vcFollow.target = slime1;
    vcFollow.offset.z = 25;

    auto& vcCone = reg_.emplace<VisionCone>(visionCone);
    vcCone.shape = ECharacterShape::Slime;

    //slime2
    auto slime2 = reg_.create();
    auto& enemyState2 = ATTACH_TO_FSM(EnemyFSM, slime2);
    enemyState2.currentState = EEnemyState::Patrolling;

    auto& enemySprite2 = reg_.emplace<Sprite>(slime2);
    AssignSprite(enemySprite2, "spritesheets:among_them_spritesheet:slime_idle_anim:1");
    enemySprite2.scale = { 1, 1 };

    auto& enemyAnimator2 = reg_.emplace<Animator>(slime2);
    AnimatorPlay(enemyAnimator2, "among_them_animations:slime_idle");

    auto& enemyTransform2 = reg_.emplace<Transform>(slime2);
    enemyTransform2.position = { 18 * 16, (-21) * 16, 1 };

    auto& enemyInput2 = reg_.emplace<InputEnemiesFile>(slime2);
    enemyInput2.pathname = "slime2.txt";
    enemyInput2.currentshape = "slime";

    auto& en2 = reg_.emplace<EnemyDescription>(slime2);
    en2.shape = ECharacterShape::Slime;

    en2.enemyIdle = false;

    auto& enemyCollision2 = reg_.emplace<SimpleCollision>(slime2);
    enemyCollision2.size = enemySprite.size;

    auto& det2 = reg_.emplace<Detection>(slime2);
    det2.SetSize(en2.detectionArea);

    reg_.emplace<MovableBody>(slime2);

    auto visionCone2 = reg_.create();

    reg_.emplace<Transform>(visionCone2);

    auto& vcSprite2 = reg_.emplace<Sprite>(visionCone2);
    AssignSprite(vcSprite2, "AmongThem:circle");
    vcSprite2.size = det2.size;

    auto& vcFollow2 = reg_.emplace<Follow>(visionCone2);
    vcFollow2.target = slime2;
    vcFollow2.offset.z = 25;

    auto& vcCone2 = reg_.emplace<VisionCone>(visionCone2);
    vcCone2.shape = ECharacterShape::Slime;


    //bat1
    auto bat1 = reg_.create();
    auto& enemyState3 = ATTACH_TO_FSM(EnemyFSM, bat1);
    enemyState3.currentState = EEnemyState::Patrolling;

    auto& enemySprite3 = reg_.emplace<Sprite>(bat1);
    AssignSprite(enemySprite3, "spritesheets:among_them_spritesheet:bat_anim:1");
    enemySprite3.scale = { 1, 1 };

    auto& enemyAnimator3 = reg_.emplace<Animator>(bat1);
    AnimatorPlay(enemyAnimator3, "among_them_animations:bat");

    auto& enemyTransform3 = reg_.emplace<Transform>(bat1);
    enemyTransform3.position = { 18 * 16, (-20) * 16, 1 };

    auto& enemyInput3 = reg_.emplace<InputEnemiesFile>(bat1);
    enemyInput3.pathname = "bat1.txt";
    enemyInput3.currentshape = "bat";

    auto& en3 = reg_.emplace<EnemyDescription>(bat1);
    en3.shape = ECharacterShape::Bat;

    en3.enemyIdle = false;

    auto& enemyCollision3 = reg_.emplace<SimpleCollision>(bat1);
    enemyCollision3.size = enemySprite3.size;

    auto& det3 = reg_.emplace<Detection>(bat1);
    det3.SetSize(en3.detectionArea);

    reg_.emplace<MovableBody>(bat1);

    auto visionCone3 = reg_.create();

    reg_.emplace<Transform>(visionCone3);

    auto& vcSprite3 = reg_.emplace<Sprite>(visionCone3);
    AssignSprite(vcSprite3, "AmongThem:circle");
    vcSprite3.size = det3.size;

    auto& vcFollow3 = reg_.emplace<Follow>(visionCone3);
    vcFollow3.target = bat1;
    vcFollow3.offset.z = 25;

    auto& vcCone3 = reg_.emplace<VisionCone>(visionCone3);
    vcCone3.shape = ECharacterShape::Bat;


    //goblin2
    auto goblin2 = reg_.create();
    auto& enemyState4 = ATTACH_TO_FSM(EnemyFSM, goblin2);
    enemyState4.currentState = EEnemyState::Patrolling;

    auto& enemySprite4 = reg_.emplace<Sprite>(goblin2);
    AssignSprite(enemySprite4, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite4.scale = { 1, 1 };

    auto& enemyAnimator4 = reg_.emplace<Animator>(goblin2);
    AnimatorPlay(enemyAnimator4, "among_them_animations:goblin_idle");

    auto& enemyTransform4 = reg_.emplace<Transform>(goblin2);
    enemyTransform4.position = { 22 * 16, (-8) * 16, 1 };

    auto& enemyInput4 = reg_.emplace<InputEnemiesFile>(goblin2);
    enemyInput4.pathname = "goblin2.txt";
    enemyInput4.currentshape = "goblin";

    auto& en4 = reg_.emplace<EnemyDescription>(goblin2);
    en4.shape = ECharacterShape::Goblin;

    en4.enemyIdle = true;

    auto& enemyCollision4 = reg_.emplace<SimpleCollision>(goblin2);
    enemyCollision4.size = enemySprite4.size;

    auto& det4 = reg_.emplace<Detection>(goblin2);
    det4.SetSize(en4.detectionArea);

    reg_.emplace<MovableBody>(goblin2);

    auto visionCone4 = reg_.create();

    reg_.emplace<Transform>(visionCone4);

    auto& vcSprite4 = reg_.emplace<Sprite>(visionCone4);
    AssignSprite(vcSprite4, "AmongThem:circle");
    vcSprite4.size = det4.size;

    auto& vcFollow4 = reg_.emplace<Follow>(visionCone4);
    vcFollow4.target = goblin2;
    vcFollow4.offset.z = 25;

    auto& vcCone4 = reg_.emplace<VisionCone>(visionCone4);
    vcCone4.shape = ECharacterShape::Goblin;

    //bat2
    auto bat2 = reg_.create();
    auto& enemyState5 = ATTACH_TO_FSM(EnemyFSM, bat2);
    enemyState5.currentState = EEnemyState::Patrolling;

    auto& enemySprite5 = reg_.emplace<Sprite>(bat2);
    AssignSprite(enemySprite5, "spritesheets:among_them_spritesheet:bat_anim:1");
    enemySprite5.scale = { 1, 1 };

    auto& enemyAnimator5 = reg_.emplace<Animator>(bat2);
    AnimatorPlay(enemyAnimator5, "among_them_animations:bat");

    auto& enemyTransform5 = reg_.emplace<Transform>(bat2);
    enemyTransform5.position = { 2 * 16, (-4) * 16, 1 };

    auto& enemyInput5 = reg_.emplace<InputEnemiesFile>(bat2);
    enemyInput5.pathname = "bat2.txt";
    enemyInput5.currentshape = "bat";

    auto& en5 = reg_.emplace<EnemyDescription>(bat2);
    en5.shape = ECharacterShape::Bat;

    en5.enemyIdle = false;

    auto& enemyCollision5 = reg_.emplace<SimpleCollision>(bat2);
    enemyCollision5.size = enemySprite5.size;

    auto& det5 = reg_.emplace<Detection>(bat2);
    det5.SetSize(en5.detectionArea);

    reg_.emplace<MovableBody>(bat2);

    auto visionCone5 = reg_.create();

    reg_.emplace<Transform>(visionCone5);

    auto& vcSprite5 = reg_.emplace<Sprite>(visionCone5);
    AssignSprite(vcSprite5, "AmongThem:circle");
    vcSprite5.size = det5.size;

    auto& vcFollow5 = reg_.emplace<Follow>(visionCone5);
    vcFollow5.target = bat2;
    vcFollow5.offset.z = 25;

    auto& vcCone5 = reg_.emplace<VisionCone>(visionCone5);
    vcCone5.shape = ECharacterShape::Bat;


    //bat3
    auto bat3 = reg_.create();
    auto& enemyState6 = ATTACH_TO_FSM(EnemyFSM, bat3);
    enemyState6.currentState = EEnemyState::Patrolling;

    auto& enemySprite6 = reg_.emplace<Sprite>(bat3);
    AssignSprite(enemySprite6, "spritesheets:among_them_spritesheet:bat_anim:1");
    enemySprite6.scale = { 1, 1 };

    auto& enemyAnimator6 = reg_.emplace<Animator>(bat3);
    AnimatorPlay(enemyAnimator6, "among_them_animations:bat");

    auto& enemyTransform6 = reg_.emplace<Transform>(bat3);
    enemyTransform6.position = { 3 * 16, (-11) * 16, 1 };

    auto& enemyInput6 = reg_.emplace<InputEnemiesFile>(bat3);
    enemyInput6.pathname = "bat3.txt";
    enemyInput6.currentshape = "bat";

    auto& en6 = reg_.emplace<EnemyDescription>(bat3);
    en6.shape = ECharacterShape::Bat;

    en6.enemyIdle = false;

    auto& enemyCollision6 = reg_.emplace<SimpleCollision>(bat3);
    enemyCollision6.size = enemySprite6.size;

    auto& det6 = reg_.emplace<Detection>(bat3);
    det6.SetSize(en6.detectionArea);

    reg_.emplace<MovableBody>(bat3);

    auto visionCone6 = reg_.create();

    reg_.emplace<Transform>(visionCone6);

    auto& vcSprite6 = reg_.emplace<Sprite>(visionCone6);
    AssignSprite(vcSprite6, "AmongThem:circle");
    vcSprite6.size = det6.size;

    auto& vcFollow6 = reg_.emplace<Follow>(visionCone6);
    vcFollow6.target = bat3;
    vcFollow6.offset.z = 25;

    auto& vcCone6 = reg_.emplace<VisionCone>(visionCone6);
    vcCone6.shape = ECharacterShape::Bat;

    //slime3
    auto slime3 = reg_.create();
    auto& enemyState7 = ATTACH_TO_FSM(EnemyFSM, slime3);
    enemyState7.currentState = EEnemyState::Patrolling;

    auto& enemySprite7 = reg_.emplace<Sprite>(slime3);
    AssignSprite(enemySprite7, "spritesheets:among_them_spritesheet:slime_idle_anim:1");
    enemySprite7.scale = { 1, 1 };

    auto& enemyAnimator7 = reg_.emplace<Animator>(slime3);
    AnimatorPlay(enemyAnimator7, "among_them_animations:slime_idle");

    auto& enemyTransform7 = reg_.emplace<Transform>(slime3);
    enemyTransform7.position = { 18 * 16, (-5) * 16, 1 };

    auto& enemyInput7 = reg_.emplace<InputEnemiesFile>(slime3);
    enemyInput7.pathname = "slime3.txt";
    enemyInput7.currentshape = "slime";

    auto& en7 = reg_.emplace<EnemyDescription>(slime3);
    en7.shape = ECharacterShape::Slime;

    en7.enemyIdle = false;

    auto& enemyCollision7 = reg_.emplace<SimpleCollision>(slime3);
    enemyCollision7.size = enemySprite7.size;

    auto& det7 = reg_.emplace<Detection>(slime3);
    det7.SetSize(en7.detectionArea);

    reg_.emplace<MovableBody>(slime3);

    auto visionCone7 = reg_.create();

    reg_.emplace<Transform>(visionCone7);

    auto& vcSprite7 = reg_.emplace<Sprite>(visionCone7);
    AssignSprite(vcSprite7, "AmongThem:circle");
    vcSprite7.size = det7.size;

    auto& vcFollow7 = reg_.emplace<Follow>(visionCone7);
    vcFollow7.target = slime3;
    vcFollow7.offset.z = 25;

    auto& vcCone7 = reg_.emplace<VisionCone>(visionCone7);
    vcCone7.shape = ECharacterShape::Slime;

    //slime4
    auto slime4 = reg_.create();
    auto& enemyState8 = ATTACH_TO_FSM(EnemyFSM, slime4);
    enemyState8.currentState = EEnemyState::Patrolling;

    auto& enemySprite8 = reg_.emplace<Sprite>(slime4);
    AssignSprite(enemySprite8, "spritesheets:among_them_spritesheet:slime_idle_anim:1");
    enemySprite8.scale = { 1, 1 };

    auto& enemyAnimator8 = reg_.emplace<Animator>(slime4);
    AnimatorPlay(enemyAnimator8, "among_them_animations:slime_idle");

    auto& enemyTransform8 = reg_.emplace<Transform>(slime4);
    enemyTransform8.position = { 16 * 16, (-5) * 16, 1 };

    auto& enemyInput8 = reg_.emplace<InputEnemiesFile>(slime4);
    enemyInput8.pathname = "slime4.txt";
    enemyInput8.currentshape = "slime";

    auto& en8 = reg_.emplace<EnemyDescription>(slime4);
    en8.shape = ECharacterShape::Slime;

    en8.enemyIdle = true;

    auto& enemyCollision8 = reg_.emplace<SimpleCollision>(slime4);
    enemyCollision8.size = enemySprite8.size;

    auto& det8 = reg_.emplace<Detection>(slime4);
    det8.SetSize(en8.detectionArea);

    reg_.emplace<MovableBody>(slime4);

    auto visionCone8 = reg_.create();

    reg_.emplace<Transform>(visionCone8);

    auto& vcSprite8 = reg_.emplace<Sprite>(visionCone8);
    AssignSprite(vcSprite8, "AmongThem:circle");
    vcSprite8.size = det8.size;

    auto& vcFollow8 = reg_.emplace<Follow>(visionCone8);
    vcFollow8.target = slime4;
    vcFollow8.offset.z = 25;

    auto& vcCone8 = reg_.emplace<VisionCone>(visionCone8);
    vcCone8.shape = ECharacterShape::Slime;
    

    //bat4
    auto bat4 = reg_.create();
    auto& enemyState9 = ATTACH_TO_FSM(EnemyFSM, bat4);
    enemyState9.currentState = EEnemyState::Patrolling;

    auto& enemySprite9 = reg_.emplace<Sprite>(bat4);
    AssignSprite(enemySprite9, "spritesheets:among_them_spritesheet:bat_anim:1");
    enemySprite9.scale = { 1, 1 };

    auto& enemyAnimator9 = reg_.emplace<Animator>(bat4);
    AnimatorPlay(enemyAnimator9, "among_them_animations:bat");

    auto& enemyTransform9 = reg_.emplace<Transform>(bat4);
    enemyTransform9.position = { 31 * 16, (-1) * 16, 1 };

    auto& enemyInput9 = reg_.emplace<InputEnemiesFile>(bat4);
    enemyInput9.pathname = "bat4.txt";
    enemyInput9.currentshape = "bat";

    auto& en9 = reg_.emplace<EnemyDescription>(bat4);
    en9.shape = ECharacterShape::Bat;

    en9.enemyIdle = false;

    auto& enemyCollision9 = reg_.emplace<SimpleCollision>(bat4);
    enemyCollision9.size = enemySprite9.size;

    auto& det9 = reg_.emplace<Detection>(bat4);
    det9.SetSize(en9.detectionArea);

    reg_.emplace<MovableBody>(bat4);

    auto visionCone9 = reg_.create();

    reg_.emplace<Transform>(visionCone9);

    auto& vcSprite9 = reg_.emplace<Sprite>(visionCone9);
    AssignSprite(vcSprite9, "AmongThem:circle");
    vcSprite9.size = det9.size;

    auto& vcFollow9 = reg_.emplace<Follow>(visionCone9);
    vcFollow9.target = bat4;
    vcFollow9.offset.z = 25;

    auto& vcCone9 = reg_.emplace<VisionCone>(visionCone9);
    vcCone9.shape = ECharacterShape::Bat;

    //goblin4
    auto goblin4 = reg_.create();
    auto& enemyState10 = ATTACH_TO_FSM(EnemyFSM, goblin4);
    enemyState8.currentState = EEnemyState::Patrolling;

    auto& enemySprite10 = reg_.emplace<Sprite>(goblin4);
    AssignSprite(enemySprite10, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite10.scale = { 1, 1 };

    auto& enemyAnimator10 = reg_.emplace<Animator>(goblin4);
    AnimatorPlay(enemyAnimator10, "among_them_animations:goblin_idle");

    auto& enemyTransform10 = reg_.emplace<Transform>(goblin4);
    enemyTransform10.position = { 25 * 16, (-5) * 16, 1 };

    auto& enemyInput10 = reg_.emplace<InputEnemiesFile>(goblin4);
    enemyInput10.pathname = "goblin4.txt";
    enemyInput10.currentshape = "goblin";

    auto& en10 = reg_.emplace<EnemyDescription>(goblin4);
    en10.shape = ECharacterShape::Goblin;

    en10.enemyIdle = true;

    auto& enemyCollision10 = reg_.emplace<SimpleCollision>(goblin4);
    enemyCollision10.size = enemySprite10.size;

    auto& det10 = reg_.emplace<Detection>(goblin4);
    det10.SetSize(en10.detectionArea);

    reg_.emplace<MovableBody>(goblin4);

    auto visionCone10 = reg_.create();

    reg_.emplace<Transform>(visionCone10);

    auto& vcSprite10 = reg_.emplace<Sprite>(visionCone10);
    AssignSprite(vcSprite10, "AmongThem:circle");
    vcSprite10.size = det10.size;

    auto& vcFollow10 = reg_.emplace<Follow>(visionCone10);
    vcFollow10.target = goblin4;
    vcFollow10.offset.z = 25;

    auto& vcCone10 = reg_.emplace<VisionCone>(visionCone10);
    vcCone10.shape = ECharacterShape::Goblin;

    //goblin3
    auto goblin3 = reg_.create();
    auto& enemyState11 = ATTACH_TO_FSM(EnemyFSM, goblin3);
    enemyState11.currentState = EEnemyState::Patrolling;

    auto& enemySprite11 = reg_.emplace<Sprite>(goblin3);
    AssignSprite(enemySprite11, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite11.scale = { 1, 1 };

    auto& enemyAnimator11 = reg_.emplace<Animator>(goblin3);
    AnimatorPlay(enemyAnimator11, "among_them_animations:goblin_idle");

    auto& enemyTransform11 = reg_.emplace<Transform>(goblin3);
    enemyTransform11.position = { 6 * 16, (-13) * 16, 1 };

    auto& enemyInput11 = reg_.emplace<InputEnemiesFile>(goblin3);
    enemyInput11.pathname = "goblin3.txt";
    enemyInput11.currentshape = "goblin";

    auto& en11 = reg_.emplace<EnemyDescription>(goblin3);
    en11.shape = ECharacterShape::Goblin;

    en11.enemyIdle = true;

    auto& enemyCollision11 = reg_.emplace<SimpleCollision>(goblin3);
    enemyCollision11.size = enemySprite11.size;

    auto& det11 = reg_.emplace<Detection>(goblin3);
    det11.SetSize(en11.detectionArea);

    reg_.emplace<MovableBody>(goblin3);

    auto visionCone11 = reg_.create();

    reg_.emplace<Transform>(visionCone11);

    auto& vcSprite11 = reg_.emplace<Sprite>(visionCone11);
    AssignSprite(vcSprite11, "AmongThem:circle");
    vcSprite11.size = det11.size;

    auto& vcFollow11 = reg_.emplace<Follow>(visionCone11);
    vcFollow11.target = goblin3;
    vcFollow11.offset.z = 25;

    auto& vcCone11 = reg_.emplace<VisionCone>(visionCone11);
    vcCone11.shape = ECharacterShape::Goblin;

    //slime5
    auto slime5 = reg_.create();
    auto& enemyState12 = ATTACH_TO_FSM(EnemyFSM, slime5);
    enemyState12.currentState = EEnemyState::Patrolling;

    auto& enemySprite12 = reg_.emplace<Sprite>(slime5);
    AssignSprite(enemySprite12, "spritesheets:among_them_spritesheet:slime_idle_anim:1");
    enemySprite12.scale = { 1, 1 };

    auto& enemyAnimator12 = reg_.emplace<Animator>(slime5);
    AnimatorPlay(enemyAnimator12, "among_them_animations:slime_idle");

    auto& enemyTransform12 = reg_.emplace<Transform>(slime5);
    enemyTransform12.position = { 6 * 16, (-4) * 16, 1 };

    auto& enemyInput12 = reg_.emplace<InputEnemiesFile>(slime5);
    enemyInput12.pathname = "slime5.txt";
    enemyInput12.currentshape = "slime";

    auto& en12 = reg_.emplace<EnemyDescription>(slime5);
    en12.shape = ECharacterShape::Slime;

    en12.enemyIdle = false;

    auto& enemyCollision12 = reg_.emplace<SimpleCollision>(slime5);
    enemyCollision12.size = enemySprite8.size;

    auto& det12 = reg_.emplace<Detection>(slime5);
    det12.SetSize(en12.detectionArea);

    reg_.emplace<MovableBody>(slime5);

    auto visionCone12 = reg_.create();

    reg_.emplace<Transform>(visionCone12);

    auto& vcSprite12 = reg_.emplace<Sprite>(visionCone12);
    AssignSprite(vcSprite12, "AmongThem:circle");
    vcSprite12.size = det12.size;

    auto& vcFollow12 = reg_.emplace<Follow>(visionCone12);
    vcFollow12.target = slime5;
    vcFollow12.offset.z = 25;

    auto& vcCone12 = reg_.emplace<VisionCone>(visionCone12);
    vcCone12.shape = ECharacterShape::Slime;

    //goblin5
    auto goblin5 = reg_.create();
    auto& enemyState13 = ATTACH_TO_FSM(EnemyFSM, goblin5);
    enemyState13.currentState = EEnemyState::Patrolling;

    auto& enemySprite13 = reg_.emplace<Sprite>(goblin5);
    AssignSprite(enemySprite13, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite13.scale = { 1, 1 };

    auto& enemyAnimator13 = reg_.emplace<Animator>(goblin5);
    AnimatorPlay(enemyAnimator13, "among_them_animations:goblin_idle");

    auto& enemyTransform13 = reg_.emplace<Transform>(goblin5);
    enemyTransform13.position = { 5 * 16, (-2) * 16, 1 };

    auto& enemyInput13 = reg_.emplace<InputEnemiesFile>(goblin5);
    enemyInput13.pathname = "goblin5.txt";
    enemyInput13.currentshape = "goblin";

    auto& en13 = reg_.emplace<EnemyDescription>(goblin5);
    en13.shape = ECharacterShape::Goblin;

    en13.enemyIdle = true;

    auto& enemyCollision13 = reg_.emplace<SimpleCollision>(goblin5);
    enemyCollision13.size = enemySprite13.size;

    auto& det13 = reg_.emplace<Detection>(goblin5);
    det13.SetSize(en13.detectionArea);

    reg_.emplace<MovableBody>(goblin5);

    auto visionCone13 = reg_.create();

    reg_.emplace<Transform>(visionCone13);

    auto& vcSprite13 = reg_.emplace<Sprite>(visionCone13);
    AssignSprite(vcSprite13, "AmongThem:circle");
    vcSprite13.size = det13.size;

    auto& vcFollow13 = reg_.emplace<Follow>(visionCone13);
    vcFollow13.target = goblin5;
    vcFollow13.offset.z = 25;

    auto& vcCone13 = reg_.emplace<VisionCone>(visionCone13);
    vcCone13.shape = ECharacterShape::Goblin;

    //goblin6
    auto goblin6 = reg_.create();
    auto& enemyState14 = ATTACH_TO_FSM(EnemyFSM, goblin6);
    enemyState14.currentState = EEnemyState::Patrolling;

    auto& enemySprite14 = reg_.emplace<Sprite>(goblin6);
    AssignSprite(enemySprite14, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite14.scale = { 1, 1 };

    auto& enemyAnimator14 = reg_.emplace<Animator>(goblin6);
    AnimatorPlay(enemyAnimator14, "among_them_animations:goblin_idle");

    auto& enemyTransform14 = reg_.emplace<Transform>(goblin6);
    enemyTransform14.position = { 9 * 16, (-2) * 16, 1 };

    auto& enemyInput14 = reg_.emplace<InputEnemiesFile>(goblin6);
    enemyInput14.pathname = "goblin6.txt";
    enemyInput14.currentshape = "goblin";

    auto& en14 = reg_.emplace<EnemyDescription>(goblin6);
    en14.shape = ECharacterShape::Goblin;

    en14.enemyIdle = true;

    auto& enemyCollision14 = reg_.emplace<SimpleCollision>(goblin6);
    enemyCollision14.size = enemySprite14.size;

    auto& det14 = reg_.emplace<Detection>(goblin6);
    det14.SetSize(en14.detectionArea);

    reg_.emplace<MovableBody>(goblin6);

    auto visionCone14 = reg_.create();

    reg_.emplace<Transform>(visionCone14);

    auto& vcSprite14 = reg_.emplace<Sprite>(visionCone14);
    AssignSprite(vcSprite14, "AmongThem:circle");
    vcSprite14.size = det14.size;

    auto& vcFollow14 = reg_.emplace<Follow>(visionCone14);
    vcFollow14.target = goblin6;
    vcFollow14.offset.z = 25;

    auto& vcCone14 = reg_.emplace<VisionCone>(visionCone14);
    vcCone14.shape = ECharacterShape::Goblin;
}

void team_game::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    auto gameCtrl = reg.create();

    auto& ctrlInput = reg.emplace<InputReceiver>(gameCtrl);
    ctrlInput.contexts.push_back("AmongThemReload");

    reg.emplace<GameController>(gameCtrl);

    // STATIC BODIES MAP
    auto mapEnt = reg.create();
	auto& map = reg.emplace<StaticBodyMap>(mapEnt);
	Engine::PutDefaultResource<StaticBodyMap>(&map);

    SetupWorld_(engine_, reg);

}

