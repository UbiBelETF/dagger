#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "core/graphics/animations.h"
#include "gameplay/common/simple_collisions.h"
#include "core/game/transforms.h"



using namespace dagger;

    
    Entity Empty(Registry& reg_, UInt32 x_, UInt32 y_);


    Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateBlankWall(Registry& reg_, UInt32 x_, UInt32 y_);

    Entity CreateWallTop(Registry& reg_, UInt32 x_, UInt32 y_);
     Entity CreateWallUpPart(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateWallDownPart(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateSideWallLeft(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateSideWallRight(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateWall1(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateWall3(Registry& reg_, UInt32 x_, UInt32 y_);


    Entity Door(Registry& reg_, UInt32 x_, UInt32 y_);

    Entity CreateWallBottom1(Registry& reg_, UInt32 x_, UInt32 y_);
    Entity CreateWallBottom6(Registry& reg_, UInt32 x_, UInt32 y_);

    Entity Hall(Registry& reg_, UInt32 x_, UInt32 y_);

