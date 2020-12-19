#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "core/graphics/animations.h"
#include "gameplay/common/simple_collisions.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/tile.h"



using namespace dagger;

struct Wall {
	bool isWall = true;
};

TileProcessor CreateObjectFunction(String name_,UInt32 depth_,Bool includeCollision_,int roomID_,float b_ ,String anim_ ,Bool wall_,UInt32 enemy_=0);

