#pragma once

#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct MovableBody
	{
		Vector2 movement{ 0, 0 };
		Vector2 size;
		Bool allowed{ true };

		void reset()
		{
			movement = { 0, 0 };
		}
	};

	class MovementSystem : public System
	{
	public:
		inline String SystemName() override { return "Movement System"; };

		void Run() override;
	};
}
