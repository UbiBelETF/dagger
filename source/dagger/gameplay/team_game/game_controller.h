#pragma once

#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct GameController
	{
		UInt32 id{ 0 };
	};

	class GameControllerSystem : public System
	{
		inline String SystemName() override { return "Game Controller System"; };

		void Run() override;
	};
}
