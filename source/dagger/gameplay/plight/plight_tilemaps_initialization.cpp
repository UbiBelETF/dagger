#include "plight_tilemaps_initialization.h"

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"

#include "gameplay/plight/plight_spikes.h"
#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_physics.h"

using namespace dagger;
using namespace plight;

Entity CreateFloor(Registry& reg_, INT32 x_, INT32 y_)
{
        Entity entity = reg_.create();
        auto& sprite = reg_.emplace<Sprite>(entity);
        sprite.position = { x_ * 16, y_ * 16, 91};
        float mod = (float)rand() / RAND_MAX;
        if (mod <= 0.8) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_1");
        }
        else if (0.825 < mod <= 0.85) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_2");
        }
        else if (0.85 < mod <= 0.875) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_3");
        }
        else if (0.875 < mod <= 0.9) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_4");
        }
        else if (0.9 < mod <= 0.925) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_5");
        }
        else if (0.925 < mod <= 0.95) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_6");
        }
        else if (0.95 < mod <= 0.975) {
            AssignSprite(sprite, "spritesheets:dungeon:floor_7");
        }
        else {
            AssignSprite(sprite, "spritesheets:dungeon:floor_8");
        }

        return entity;   
}

Entity CreateBlackBackground(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 90 };
    AssignSprite(sprite, "EmptyWhitePixel");
    sprite.color = { 0.133f,0.133f,0.133f,1.f };
    sprite.size = { 16,16 };
    return entity;
}

Entity CreatePhysicsObject(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& collision = reg_.emplace<PlightCollision>(entity);
    collision.size.x = 16;
    collision.size.y = 16;
    auto& t = reg_.emplace<Transform>(entity);
    t.position = { x_ * 16, y_ * 16, 90 };
    auto& physic_object = reg_.emplace<PhysicsObject>(entity);
    physic_object.collision_groups.push_back(1);
    return entity;
}

Entity CreateEmpty(Registry& reg_, INT32 x_, INT32 y_)
{
    return Entity();
}

Entity CreateWallSideTopLeft(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_top_left");
    
    return entity;
}

Entity CreateWallSideTopRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_top_right");
    return entity;
}

Entity CreateWallCornerBottomLeft(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 85 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_bottom_left");
    return entity;
}

Entity CreateWallCornerBottomLeftFront(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 79 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_bottom_left");
    return entity;
}

Entity CreateWallCornerBottomRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 85 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_bottom_right");
    return entity;
}
Entity CreateWallCornerBottomRightColumn(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 75 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_bottom_right");
    return entity;
}

Entity CreateWallCornerMidRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 79 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_inner_corner_mid_rigth");
    return entity;
}

Entity CreateWallSideMidLeft(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_mid_left");
    return entity;
}
Entity CreateWallCornerRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_right");
    return entity;
}

Entity CreateWallCornerLeft(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 79 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_corner_left");
    return entity;
}

Entity CreateWallGoo(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_goo");

    Entity entity2 = reg_.create();
    auto& sprite2 = reg_.emplace<Sprite>(entity2);
    sprite2.position = { x_ * 16, (y_-1) * 16, 89 };
    AssignSprite(sprite2, "spritesheets:dungeon:wall_goo_base");
    return entity;
}

Entity CreateBlueFountain(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 87 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_fountain_mid_blue_anim:1");
    auto& animator = reg_.emplace<Animator>(entity);
    AnimatorPlay(animator, "Plight:environment:FOUNTAIN_MID_BLUE");

    Entity entity2 = reg_.create();
    auto& sprite2 = reg_.emplace<Sprite>(entity2);
    sprite2.position = { x_ * 16, (y_-1)* 16, 87 };
    AssignSprite(sprite2, "spritesheets:dungeon:wall_fountain_basin_blue_anim:1");
    auto& animator2 = reg_.emplace<Animator>(entity2);
    AnimatorPlay(animator2, "Plight:environment:FOUNTAIN_BASIN_BLUE");

    Entity entity3 = reg_.create();
    auto& sprite3= reg_.emplace<Sprite>(entity3);
    sprite3.position = { x_ * 16, (y_ + 1) * 16, 87 };
    AssignSprite(sprite3, "spritesheets:dungeon:wall_fountain_top");


    return entity;
}

Entity CreateRedFountain(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 87 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_fountain_mid_red_anim:1");
    auto& animator = reg_.emplace<Animator>(entity);
    AnimatorPlay(animator, "Plight:environment:FOUNTAIN_MID_RED");

    Entity entity2 = reg_.create();
    auto& sprite2 = reg_.emplace<Sprite>(entity2);
    sprite2.position = { x_ * 16, (y_ - 1) * 16, 87 };
    AssignSprite(sprite2, "spritesheets:dungeon:wall_fountain_basin_red_anim:1");
    auto& animator2 = reg_.emplace<Animator>(entity2);
    AnimatorPlay(animator2, "Plight:environment:FOUNTAIN_BASIN_RED");

    Entity entity3 = reg_.create();
    auto& sprite3 = reg_.emplace<Sprite>(entity3);
    sprite3.position = { x_ * 16, (y_ + 1) * 16, 87 };
    AssignSprite(sprite3, "spritesheets:dungeon:wall_fountain_top");
    return entity;
}

Entity CreateFloorSpikes(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& spikes = reg_.emplace<PlightSpikes>(entity);

    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:floor_spikes_anim:1");

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = sprite.position;

    auto& collision = reg_.emplace<PlightCollision>(entity);
    auto& animator = reg_.emplace<Animator>(entity);
    ATTACH_TO_FSM(PlightSpikesFSM, entity);

    return entity;
}

Entity CreateFrontWall(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 88 };
    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.33) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_left");
    }
    else if (0.33 < mod <= 0.66) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_mid");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_right");
    }
    return entity;
}
Entity CreateFrontWallFront(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 78 };
    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.33) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_left");
    }
    else if (0.33 < mod <= 0.66) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_mid");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_right");
    }
    return entity;
}
Entity CreateFrontWallColumn(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 75 };
    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.33) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_left");
    }
    else if (0.33 < mod <= 0.66) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_mid");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:wall_top_right");
    }
    return entity;
}
Entity CreateRoof(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 75 };
    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.33) {
        AssignSprite(sprite, "spritesheets:dungeon:roof_left");
    }
    else if (0.33 < mod <= 0.66) {
        AssignSprite(sprite, "spritesheets:dungeon:roof_mid");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:roof_right");
    }
    return entity;
}

Entity CreateWallMid(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
   
    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.33) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid1");
    }
    else if (0.33 < mod <= 0.66) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid2");
    }
    else if (0.66 < mod <= 98.0) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid3");
    }
    else if (0.98 < mod <= 0.99) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_hole_1");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:wall_hole_2");
    }
    return entity;
}

Entity CreateWallMidFront(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 79 };

    float mod = (float)rand() / RAND_MAX;
    if (mod <= 0.32) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid1");
    }
    else if (0.32 < mod <= 0.64) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid2");
    }
    else if (0.64 < mod <= 0.98) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_mid3");
    }
    else if (0.98 < mod <= 0.99) {
        AssignSprite(sprite, "spritesheets:dungeon:wall_hole_1");
    }
    else {
        AssignSprite(sprite, "spritesheets:dungeon:wall_hole_2");
    }
    return entity;
}

Entity CreateWallSideFrontLeft(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_front_left");

    return entity;
}

Entity CreateWallSideFrontRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_front_right");

    return entity;
}

Entity CreateSideWallMidRight(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 85 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_side_mid_right");
    return entity;
}

Entity CreateWallColumn(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 90 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_coulmn_base");

   
    Entity entity2 = reg_.create();
    auto& sprite2 = reg_.emplace<Sprite>(entity2);
    sprite2.position = { x_ * 16, (y_+1) * 16, 79 };
    AssignSprite(sprite2, "spritesheets:dungeon:column_mid");

    Entity entity3 = reg_.create();
    auto& sprite3 = reg_.emplace<Sprite>(entity3);
    sprite3.position = { x_ * 16, (y_ + 2) * 16, 79 };
    AssignSprite(sprite3, "spritesheets:dungeon:column_top");

    return entity;
}

Entity CreateWallBannerBlue(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_banner_blue");
    return entity;
}

Entity CreateWallBannerRed(Registry& reg_, INT32 x_, INT32 y_)
{
    Entity entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    sprite.position = { x_ * 16, y_ * 16, 89 };
    AssignSprite(sprite, "spritesheets:dungeon:wall_banner_red");
    return entity;
}
