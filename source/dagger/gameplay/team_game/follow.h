#pragma once

#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct Follow
	{
		Entity target;
		Vector3 offset{ 0, 0, 0 };
	};

	class FollowSystem : public System
	{
		inline String SystemName() override { return "Follow System"; };

		void Run() override;
	};
}
