#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "core/graphics/animations.h"
#include "gameplay/common/simple_collisions.h"
#include "core/game/transforms.h"
#include "tile.h"



using namespace dagger;


TileProcessor CreateObjectFunction(String name_,UInt32 depth_,Bool includeCollision_,int roomID_,float b_ ,String anim_ );



