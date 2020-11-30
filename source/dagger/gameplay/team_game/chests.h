#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	struct Chest
	{
		Float32 hpRestored;
		bool empty;
	};

	void CreateChest(Vector2 position, Float32 hpRestored);
}