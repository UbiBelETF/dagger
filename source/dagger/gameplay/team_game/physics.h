#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct StaticBody
	{
		Vector2 size;
	};

	struct StaticBodyMap
	{
		std::map<Pair<SInt32, SInt32>, Entity> map;

		void put(SInt32 x_, SInt32 y_, Entity& entity_)
		{
			map[pair(x_, y_)] = entity_;
		}

		Entity get(SInt32 x_, SInt32 y_)
		{
			return map[pair(x_, y_)];
		}
	};

	class PhysicsSystem : public System
	{
	public:
		inline String SystemName() override { return "Physics System"; }

		void Run() override;
	};
}
