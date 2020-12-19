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
		std::map<Pair<SInt32, SInt32>, StaticBody> map;

		void put(SInt32 x_, SInt32 y_, StaticBody& body_)
		{
			map[pair(x_, y_)] = body_;
		}

		StaticBody get(SInt32 x_, SInt32 y_)
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
