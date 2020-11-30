#include "level_generator.h"

#include "core/graphics/sprite.h"

using namespace dagger;

std::vector<Entity> level_generator::jovica::CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    int type = 1 + rand() % 10;
    AssignSprite(sprite, fmt::format("spritesheets:among_them_tilemap:floor_{}", type));

    return { entity };
}

std::vector<Entity> level_generator::jovica::Nothing(Registry& reg_, SInt32 x_, SInt32 y_)
{
    return {};
}

std::vector<Entity> level_generator::jovica::CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    int type = 1 + rand() % 6;
    AssignSprite(sprite, fmt::format("spritesheets:among_them_tilemap:wall_{}", type));

    Entity top = reg_.create();
    auto& topSprite = reg_.emplace<Sprite>(top);
    topSprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

    AssignSprite(topSprite, "spritesheets:among_them_tilemap:wall_top");

    return { entity, top };
}

std::vector<Entity> level_generator::jovica::CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_bottom");

    return { entity };
}

std::vector<Entity> level_generator::jovica::CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_top_left");

    return { entity };
}

std::vector<Entity> level_generator::jovica::CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_top_right");

    return { entity };
}

std::vector<Entity> level_generator::jovica::CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_top_left");

    Entity top = reg_.create();
    auto& topSprite = reg_.emplace<Sprite>(top);
    topSprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

    AssignSprite(topSprite, "spritesheets:among_them_tilemap:wall_top_inner_left");

    return { entity, top };
}

std::vector<Entity> level_generator::jovica::CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 30 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_top_right");

    Entity top = reg_.create();
    auto& topSprite = reg_.emplace<Sprite>(top);
    topSprite.position = { x_ * 16, (y_ + 1) * 16, 29 };

    AssignSprite(topSprite, "spritesheets:among_them_tilemap:wall_top_inner_right");

    return { entity, top };
}

std::vector<Entity> level_generator::jovica::CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 29 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_bottom_inner_left");

    return { entity };
}

std::vector<Entity> level_generator::jovica::CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 29 };

    AssignSprite(sprite, "spritesheets:among_them_tilemap:wall_bottom_inner_right");

    return { entity };
}
