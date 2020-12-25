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


 
Entity level_generator::CreateTile(Registry& reg_, SInt32 x_, SInt32 y_, SInt32 z_, String name_)
{
    Entity entity = reg_.create();

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = { x_ * 16, y_ * 16, z_ };

    auto& sprite = reg_.emplace<Sprite>(entity);
    AssignSprite(sprite, name_);

    return entity;
}

void level_generator::EmplaceCollider(Registry& reg_, Entity entity_, SInt32 x_, SInt32 y_)
{
    auto& collider = reg_.emplace<StaticBody>(entity_);
    collider.size = { 32, 32 };
    Engine::GetDefaultResource<StaticBodyMap>()->put(x_, y_, entity_);
    Logger::info("Inserted into map on [{}, {}]", x_, y_);
}

void level_generator::CreateCollider(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = { x_ * 16, y_ * 16, 30 };

    EmplaceCollider(reg_, entity, x_, y_);
}

Sequence<Entity> level_generator::CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 10;
    Entity entity = CreateTile(reg_, x_, y_, 30, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { entity };
}

Sequence<Entity> level_generator::Nothing(Registry& reg_, SInt32 x_, SInt32 y_)
{
    return {};
}

Sequence<Entity> level_generator::CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
{

    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top");

    return { entity, top };
}

Sequence<Entity> level_generator::CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom");
    CreateCollider(reg_, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_left");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_right");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_right");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_left");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_right");
    EmplaceCollider(reg_, entity, x_, y_);

    return { entity };
}

Sequence<Entity> level_generator::CreateBottomLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity right = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    EmplaceCollider(reg_, right, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_right");

    return { right, top };
}

Sequence<Entity> level_generator::CreateBottomRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity left = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_right");
    EmplaceCollider(reg_, left, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_left");

    return { left, top };
}

Sequence<Entity> level_generator::CreateTopLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left_2");

    return { entity, top };
}

Sequence<Entity> level_generator::CreateTopRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_right_2");

    return { entity, top };
}

Sequence<Entity> level_generator::CreateBottomLeftConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    Entity right = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_left");
    EmplaceCollider(reg_, right, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_right");

    return { right, top };
}

Sequence<Entity> level_generator::CreateBottomRightConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    Entity left = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_bottom_inner_right");
    EmplaceCollider(reg_, left, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_bottom_left");

    return { left, top };
}

Sequence<Entity> level_generator::CreateTopLeftConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left_2");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { entity, top, peak };
}

Sequence<Entity> level_generator::CreateTopRightConcWallS(Registry & reg_, SInt32 x_, SInt32 y_)
{
    UInt32 type = 1 + rand() % 6;
    Entity entity = CreateTile(reg_, x_, y_, 20, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));
    EmplaceCollider(reg_, entity, x_, y_);
    Entity top = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_right_2");
    Entity peak = CreateTile(reg_, x_, y_ + 2, 0, "spritesheets:among_them_tilemap:wall_top_inner_right");

    return { entity, top, peak };
}


Sequence<Entity> level_generator::CreateIdleGoblin(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateKey(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateBigTable(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateBookShelfOnTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreatePrisonerOnTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateChest(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateGreenBanner(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateRedBanner(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateTorch(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateBarrel(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::CreateSmallTable(Registry& reg_, SInt32 x_, SInt32 y_)
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

Sequence<Entity> level_generator::TopLeftSide(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity top = CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_tilemap:wall_top_left");
    Entity peak = CreateTile(reg_, x_, y_ + 1, 0, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { top, peak };
}
