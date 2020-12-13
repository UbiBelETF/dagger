#pragma once

#include "core/engine.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    class PlayerStatsUISystem : public System
    {
    public:
		inline String SystemName() override {
			return "Player Stats UI System";
		};

		void Run() override;
    };
}
