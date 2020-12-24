#pragma once


#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct CounterType
	{

		Bool type = false;
	};
    class DeathsCounterSystem
        : public System
    {
        inline String SystemName() override {
            return "Death Counter System";
        };

        void Run() override;
    };
}