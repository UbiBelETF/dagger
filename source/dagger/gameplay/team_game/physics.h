#pragma once

#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct StaticBody
	{
		Vector2 size;
	};

	class PhysicsSystem : public System
	{
	public:
		inline String SystemName() override { return "Physics System"; }

		void Run() override;
	};
}
