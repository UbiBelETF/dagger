#pragma once

#include "core/core.h"

namespace level_generator
{
	namespace jovica
	{
        Entity CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity Nothing(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateTopWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateBottomWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateTopLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateTopRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateBottomLeftWall(Registry& reg_, SInt32 x_, SInt32 y_);
        Entity CreateBottomRightWall(Registry& reg_, SInt32 x_, SInt32 y_);
	}
}
