#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;
    
    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, fmt::format("spritesheets:lab:floor_{}", 1 + (rand() % 6)));
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }

    Entity CreateWallTop(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_2");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
     Entity CreateWallUpPart(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_5");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
    Entity CreateWallDownPart(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_top_second_left");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
    Entity CreateSideWallLeft(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_4");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
    Entity CreateSideWallRight(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_6");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
    Entity CreateWall1(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_1");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }
    Entity CreateWall3(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_3");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        return entity;
    }