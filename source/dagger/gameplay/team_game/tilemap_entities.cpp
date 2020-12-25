#include "gameplay/team_game/tilemap_entities.h"


#include "core/core.h"
#include "core/system.h"
#include "gameplay/team_game/player_controller_fsm.h"
#include "gameplay/team_game/enemy.h"
#include "gameplay/team_game/collectables.h"

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
        else
        {
            AssignSprite(sprite, name_);
            if (name_ == "Heart")
            {
                sprite.scale *= 0.4f;
                auto& heart = reg_.emplace<Heart>(entity);
            }
            else if (name_ == "SilverHeart")
            {
                sprite.scale *= 0.4f;
                auto& silverHeart = reg_.emplace<SilverHeart>(entity);
            }
        }
        if(includeCollision_)
        {
            auto& col=reg_.emplace<SimpleCollision>(entity);
            if (enemy_ <= 5 && enemy_ > 0)
            {
                col.size = { 17, 18 };
                col.pivot.y += 0.35f;
                col.pivot.x += 0.1;
            }
            else if (enemy_ == 6)
            {
                col.size = { 20, 15 };
                col.pivot.y -= 0.1;
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
            if (enemy_ <= 5)
            {
                auto& skeleton = reg_.emplace<Skeleton>(entity);
                switch (enemy_)
                {
                case 1: skeleton.type = horizontal; break;
                case 2: skeleton.type = vertical; break;
                case 3: skeleton.type = follower; break;
                case 4: skeleton.type = boss1; break;
                case 5: skeleton.type = boss2; break;
                }
            }
            else
            {
                auto& slime = reg_.emplace<Slime>(entity);
            }
        }

        return entity;

    };
}

