#include "level_generator.h"

#include "core/engine.h"

#include "core/graphics/animation.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/team_game/physics.h"
#include "gameplay\team_game\character_controller.h"
#include "core\input\inputs.h"
#include "gameplay\team_game\movement.h"
#include "gameplay\team_game\detection.h"
#include "gameplay\team_game\follow.h"
#include "gameplay\team_game\remote_animation.h"
#include "gameplay\team_game\door_interaction.h"
#include "gameplay\common\simple_collisions.h"
#include "gameplay\team_game\enemy.h"
#include <iostream>
#include <gameplay\team_game\key.h>
#include <gameplay\team_game\vision_cone.h>

using namespace dagger;
using namespace team_game;

Sequence<Entity> level_generator::smiljana::CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30};
    AssignSprite(sprite, "spritesheets:among_them_tilemap:floor_1");
    auto& entityAnimator = reg_.emplace<Animator>(entity);
   
    return { entity };
}

Sequence<Entity> level_generator::smiljana::CreateWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30};
    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_1");
    jovica::EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

 
Entity level_generator::jovica::CreateTile(Registry& reg_, SInt32 x_, SInt32 y_, SInt32 z_, String name_)
{
    Entity entity = reg_.create();

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = { x_ * 16, y_ * 16, z_ };

    auto& sprite = reg_.emplace<Sprite>(entity);
    AssignSprite(sprite, name_);

    return entity;
}

void level_generator::jovica::EmplaceCollider(Registry& reg_, Entity entity_, SInt32 x_, SInt32 y_)
{
    auto& collider = reg_.emplace<StaticBody>(entity_);
    collider.size = { 32, 32 };
    Engine::GetDefaultResource<StaticBodyMap>()->put(x_, y_, entity_);
    Logger::info("Inserted into map on [{}, {}]", x_, y_);
}

void level_generator::jovica::CreateCollider(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = { x_ * 16, y_ * 16, 30 };

    EmplaceCollider(reg_, entity, x_, y_);
}

Sequence<Entity> level_generator::jovica::CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { entity };
}

Sequence<Entity> level_generator::jovica::Nothing(Registry& reg_, SInt32 x_, SInt32 y_)
{
    return {};
}

Sequence<Entity> level_generator::jovica::CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
{

    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top");

    return { entity, top };
}

Sequence<Entity> level_generator::jovica::CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom");
    CreateCollider(reg_, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::jovica::CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::jovica::CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::jovica::CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_left");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::jovica::CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_right");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_right");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::jovica::CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_left");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::jovica::CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_right");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::jovica::CreateBottomLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity right = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, right, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_right");

    return { right, top };
}

Sequence<Entity> level_generator::jovica::CreateBottomRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity left = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, left, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_left");

    return { left, top };
}

Sequence<Entity> level_generator::jovica::CreateTopLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left_2");

    return { entity, top };
}

Sequence<Entity> level_generator::jovica::CreateTopRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_right_2");

    return { entity, top };
}

Sequence<Entity> level_generator::jovica::CreateBottomLeftConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    Entity right = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_left");
    EmplaceCollider(reg_, right, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_right");

    return { right, top };
}

Sequence<Entity> level_generator::jovica::CreateBottomRightConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    Entity left = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_right");
    EmplaceCollider(reg_, left, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_left");

    return { left, top };
}

Sequence<Entity> level_generator::jovica::CreateTopLeftConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left_2");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::jovica::CreateTopRightConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_right_2");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_right");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::jovica::CreatePlayer(Registry& reg_, SInt32 x_, SInt32 y_)
{
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
    playerTransform.position = { x_ * 16, y_ * 16, 1 };
    auto& playerInput = reg_.get_or_emplace<InputReceiver>(player);
    playerInput.contexts.push_back("AmongThemInput");
    auto& controller = reg_.emplace<CharacterController>(player);
    reg_.emplace<MovableBody>(player);

    auto& heroDet = reg_.emplace<Detection>(player);
    heroDet.SetSize({ 2, 2 });

    auto& playerCollision = reg_.emplace<SimpleCollision>(player);
    playerCollision.size = playerSprite.size;
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

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));
    return {player, entity};
}

Sequence<Entity> level_generator::jovica::CreateDoor(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto door = reg_.create();

    auto& doorSprite = reg_.emplace<Sprite>(door);
    AssignSprite(doorSprite, "spritesheets:among_them_spritesheet:door_open_anim:1");
    doorSprite.scale = { 1, 1 };

    auto& doorAnimator = reg_.emplace<Animator>(door);

    auto& doorTransform = reg_.emplace<Transform>(door);
    doorTransform.position = { x_ * 16, y_ * 16 + 8, 1 };

    auto& doorCollision = reg_.emplace<SimpleCollision>(door);
    doorCollision.size = doorSprite.size;

    SInt32 x = ((SInt32)doorTransform.position.x + 8) / 16;
    SInt32 y = ((SInt32)doorTransform.position.y - 8) / 16;

    auto& collider = reg_.emplace<StaticBody>(door);
    collider.size = doorSprite.size;
    Engine::GetDefaultResource<StaticBodyMap>()->put(x_, y_, door);

    reg_.emplace<Door>(door);
    return { door };
}

Sequence<Entity> level_generator::jovica::CreateIdleGoblin(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto enemy = reg_.create();
    auto& enemyState = ATTACH_TO_FSM(EnemyFSM, enemy);
    enemyState.currentState = EEnemyState::Patrolling;

    auto& enemySprite = reg_.emplace<Sprite>(enemy);
    AssignSprite(enemySprite, "spritesheets:among_them_spritesheet:goblin_idle_anim:1");
    enemySprite.scale = { 1, 1 };

    auto& enemyAnimator = reg_.emplace<Animator>(enemy);
    AnimatorPlay(enemyAnimator, "among_them_animations:goblin_idle");

    auto& enemyTransform = reg_.emplace<Transform>(enemy);
    enemyTransform.position = { x_ * 16, y_ * 16, 1 };

    auto& enemyInput = reg_.emplace<InputEnemiesFile>(enemy);
    enemyInput.pathname = "pathidlegoblin1.txt";
    enemyInput.currentshape = "goblin";

    auto& en = reg_.emplace<EnemyDescription>(enemy);
    en.shape = ECharacterShape::Goblin;

    en.enemyIdle = true;

    auto& enemyCollision = reg_.emplace<SimpleCollision>(enemy);
    enemyCollision.size = enemySprite.size;

    auto& det = reg_.emplace<Detection>(enemy);
    det.SetSize(en.detectionArea);

    reg_.emplace<MovableBody>(enemy);

    auto visionCone14 = reg_.create();

    reg_.emplace<Transform>(visionCone14);

    auto& vcSprite14 = reg_.emplace<Sprite>(visionCone14);
    AssignSprite(vcSprite14, "AmongThem:circle");
    vcSprite14.size = det.size;

    auto& vcFollow14 = reg_.emplace<Follow>(visionCone14);
    vcFollow14.target = enemy;
    vcFollow14.offset.z = 25;

    auto& vcCone14 = reg_.emplace<VisionCone>(visionCone14);
    vcCone14.shape = ECharacterShape::Goblin;

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return {enemy, entity};
}

Sequence<Entity> level_generator::jovica::CreateKey(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto key = reg_.create();

    auto& keySprite = reg_.emplace<Sprite>(key);
    AssignSprite(keySprite, "spritesheets:among_them_tilemap:key");
    keySprite.scale = { 1, 1 };

    auto& keyTransform = reg_.emplace<Transform>(key);
    keyTransform.position = { x_ * 16, y_ * 16, 1 };

    auto& keyCollision = reg_.emplace<SimpleCollision>(key);
    keyCollision.size = keySprite.size;

    reg_.emplace<Key>(key);
    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { key , entity};
}

Sequence<Entity> level_generator::jovica::CreateBigTable(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto bigTable = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(bigTable);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:big_table");
    sprite.scale = { 1,1 };
    sprite.rotation = 90;

    auto& transform = reg_.emplace<Transform>(bigTable);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, bigTable, x_, y_);

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { bigTable, entity };
}

Sequence<Entity> level_generator::jovica::CreateBookShelfOnTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto bookShelf = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(bookShelf);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:bookshelf");
    sprite.scale = { 1,1 };

    auto& transform = reg_.emplace<Transform>(bookShelf);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, bookShelf, x_, y_);

   Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top");

    return { bookShelf, top };
}

Sequence<Entity> level_generator::jovica::CreatePrisonerOnTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto prisoner = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(prisoner);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:prisoner");
    sprite.scale = { 1,1 };

    auto& transform = reg_.emplace<Transform>(prisoner);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, prisoner, x_, y_);

    Sequence<Entity> ent = CreateTopWall(reg_, x_, y_);
  
    ent.push_back(prisoner);

    return ent;
}

Sequence<Entity> level_generator::jovica::CreateChest(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto chest = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(chest);
    AssignSprite(sprite, "spritesheets:among_them_spritesheet:chest_shine_anim:1");
    sprite.scale = { 1,1 };

    auto& transform = reg_.emplace<Transform>(chest);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, chest, x_, y_);

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { chest, entity };
}

Sequence<Entity> level_generator::jovica::CreateGreenBanner(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto gBanner = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(gBanner);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:green_banner");

    auto& transform = reg_.emplace<Transform>(gBanner);
    transform.position = { x_ * 16, y_ * 16, 1 };

    Sequence<Entity> ent = CreateTopWall(reg_, x_, y_);
    ent.push_back(gBanner);

    return ent;
}

Sequence<Entity> level_generator::jovica::CreateRedBanner(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto rBanner = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(rBanner);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:red_banner");

    auto& transform = reg_.emplace<Transform>(rBanner);
    transform.position = { x_ * 16, y_ * 16, 1 };

    Sequence<Entity> ent = CreateTopWall(reg_, x_, y_);
    ent.push_back(rBanner);

    return ent;
}

Sequence<Entity> level_generator::jovica::CreateTorch(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto torch = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(torch);
    AssignSprite(sprite, "spritesheets:among_them_spritesheet:torch_anim:1");

    auto& transform = reg_.emplace<Transform>(torch);
    transform.position = { x_ * 16, y_ * 16, 1 };

    auto& animator = reg_.emplace<Animator>(torch);
    AnimatorPlay(animator, "among_them_animations:torch");

    Sequence<Entity> ent = CreateTopWall(reg_, x_, y_);
    ent.push_back(torch);

    return ent;
}

Sequence<Entity> level_generator::jovica::CreateBarrel(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto barrel = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(barrel);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:barrel");
    sprite.scale = { 1,1 };

    auto& transform = reg_.emplace<Transform>(barrel);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, barrel, x_, y_);

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { barrel, entity };
}

Sequence<Entity> level_generator::jovica::CreateSmallTable(Registry& reg_, SInt32 x_, SInt32 y_)
{
    auto smallTable = reg_.create();

    auto& sprite = reg_.emplace<Sprite>(smallTable);
    AssignSprite(sprite, "spritesheets:among_them_tilemap:small_table");
    sprite.scale = { 1,1 };

    auto& transform = reg_.emplace<Transform>(smallTable);
    transform.position = { x_ * 16, y_ * 16, 1 };

    EmplaceCollider(reg_, smallTable, x_, y_);

    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { smallTable, entity };
}

Sequence<Entity> level_generator::jovica::TopLeftSide(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity top = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    Entity peak = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { top, peak };
}
