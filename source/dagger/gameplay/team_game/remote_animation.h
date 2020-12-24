#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct AnimationExecutor
	{
		Bool* source{ nullptr };
		String startingSpriteName{ "" };
		String animationName{ "" };
	};

	class RemoteAnimationSystem : public System
	{
		inline String SystemName() override { return "Remote Animation System"; };

		void Run() override;
	};
}
