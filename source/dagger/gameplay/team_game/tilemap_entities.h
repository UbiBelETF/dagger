#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "core/graphics/animations.h"
#include "gameplay/common/simple_collisions.h"
#include "core/game/transforms.h"

using namespace dagger;
    
    

    Entity Empty(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        return entity;
    }


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
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        
        return entity;
    }
     Entity CreateWallUpPart(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_5");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }
    Entity CreateWallDownPart(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_top_second_left");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        return entity;
    }
    Entity CreateSideWallLeft(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_4");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        return entity;
    }
    Entity CreateSideWallRight(Registry& reg_, UInt32 x_, UInt32 y_)
    {   
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_6");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        return entity;
    }
    Entity CreateWall1(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_1");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }
    Entity CreateWall3(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_3");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }


    Entity Door(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        auto& anim = reg_.get_or_emplace<Animator>(entity);
        AssignSprite(sprite, "lab:door:lab anims_Animation 5_00");  
        AnimatorPlay(anim, "lab:door");       
        sprite.position = { x_ * 16-125.0f, y_ * 16-107.1f, 30 };
        sprite.scale = { 1, 1 };
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        return entity;
    }

    Entity CreateWallBottom1(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_bottom_1");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }
    Entity CreateWallBottom6(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:wall_bottom_6");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& idd=reg_.emplace<lab::CollideW>(entity);
        idd.id=0;
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }

    Entity Hall(Registry& reg_, UInt32 x_, UInt32 y_)
    {
        Entity entity = reg_.create();
        auto& idd=reg_.emplace<lab::NextLvl>(entity);
        idd.id=1;
        auto& sprite = reg_.emplace<Sprite>(entity);
        AssignSprite(sprite, "spritesheets:lab:floor_1");
        sprite.position = { x_ * 16-125.0f, y_ * 16-100.0f, 30 };
        auto& col=reg_.emplace<SimpleCollision>(entity);
        col.size={sprite.size.x/2,sprite.size.y/2};
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position=sprite.position;
        return entity;
    }

    