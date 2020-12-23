#pragma once

#include "core/core.h"

namespace level_generator
{
    namespace smiljana
    {
        Sequence<Entity> CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateWall(Registry& reg_, SInt32 x_, SInt32 y_);
    }

	namespace jovica
	{
        Entity CreateTile(Registry& reg_, SInt32 x_, SInt32 y_, SInt32 z_, String name_);

        void EmplaceCollider(Registry& reg_, Entity entity_, SInt32 x_, SInt32 y_);
        void CreateCollider(Registry& reg_, SInt32 x_, SInt32 y_);
       
        Sequence<Entity> CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> Nothing(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopLeftConcWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopRightConcWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomLeftConcWallS(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateBottomRightConcWallS(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopLeftConcWallS(Registry& reg_, SInt32 x_, SInt32 y_);
        Sequence<Entity> CreateTopRightConcWallS(Registry& reg_, SInt32 x_, SInt32 y_);
	}
}
