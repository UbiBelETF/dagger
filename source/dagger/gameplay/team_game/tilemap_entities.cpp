#include "tilemap_entities.h"


#include "core/core.h"
#include "core/system.h"
#include "player_controller_fsm.h"
#include "gameplay/team_game/enemy.h"

using namespace dagger;
using namespace lab;

TileProcessor CreateObjectFunction(String name_,UInt32 depth_,Bool includeCollision_,int roomID_,float b_,String anim_ ,Bool wall_,UInt32 enemy_)
{
    return[name_,depth_,includeCollision_ ,roomID_,b_,anim_,wall_,enemy_](Registry& reg_, UInt32 x_, UInt32 y_)->Entity
    {
        Entity entity=reg_.create();
        if(name_=="")
        {
            return entity;
        }
        auto& sprite =reg_.emplace<Sprite>(entity);   
        if(name_=="create_floor") AssignSprite(sprite,fmt::format("spritesheets:lab:floor_{}", 1 + (rand() % 6)));
        else AssignSprite(sprite,name_);
        if(includeCollision_)
        {
            auto& col=reg_.emplace<SimpleCollision>(entity);
            if (enemy_)
            {
                col.size = { 18, 18 };
                col.pivot.y += 0.35f;
                col.pivot.x += 0.1;
            }
            else
                col.size={sprite.size.x/2,sprite.size.y/2};  
        }
        auto& trn=reg_.emplace<Transform>(entity);
        trn.position={ x_ * 16-125.0f, y_ * 16-b_, depth_ };

        if(roomID_>=0)
        {
            auto& idd=reg_.emplace<NextLvl>(entity);
            idd.id=roomID_;
        }
        if(anim_!="")
        {
            auto& anim = reg_.get_or_emplace<Animator>(entity);
            AnimatorPlay(anim, anim_); 
        }
        if (wall_)
        {
            auto& wall = reg_.emplace<Wall>(entity);
        }
        if(enemy_)
        {
            auto& skeleton = reg_.emplace<Skeleton>(entity);
            switch (enemy_)
            {
            case 1: skeleton.type = horizontal; break;
            case 2: skeleton.type = vertical; break;
            default: skeleton.type = follower; break;
            }
        }

        return entity;

    };
}

