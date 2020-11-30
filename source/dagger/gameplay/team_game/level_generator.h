#pragma once

#include "core/core.h"

namespace level_generator
{
	namespace jovica
	{
        std::vector<Entity> CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> Nothing(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        std::vector<Entity> CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
	}
}
