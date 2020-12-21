#include "level_generator.h"

#include "core/engine.h"

#include "core/graphics/animation.h"

#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

#include "gameplay/team_game/physics.h"

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

    return { entity };
}

Sequence<Entity> level_generator::smiljana::CreateDoor(Registry& reg_, SInt32 x_, SInt32 y_)
{  
    Entity entity = jovica::CreateTile(reg_, x_, y_, 0, "spritesheets:among_them_spritesheet:door_open_anim:14");
    jovica::CreateCollider(reg_, x_, y_);
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
