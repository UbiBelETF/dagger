#pragma once

#include "core/system.h"

#include "gameplay/team_game/character_controller.h"

using namespace dagger;

namespace team_game
{
	struct VisionCone
	{
		ECharacterShape shape;
	};

	class VisionConeSystem : public System
	{
		inline String SystemName() override { return "Vision Cone System"; };

		void Run() override;
	};
}
