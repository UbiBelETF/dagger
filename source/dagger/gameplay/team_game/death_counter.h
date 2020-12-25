#pragma once


#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct CounterType
	{
		Bool type = false;
        Bool visible = true;
	};
    struct BarOrCredits
    {
        Bool bar = true;
        Float32 timer{ 2 };
        Bool once = true;
    };
    static UINT32 score{ 0 };
    class DeathsCounterSystem
        : public System
    {
        inline String SystemName() override {
            return "Death Counter System";
        };

        void Run() override;
    };
}