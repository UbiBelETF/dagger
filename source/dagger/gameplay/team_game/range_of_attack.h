#pragma once

#include "core/system.h"

using namespace dagger;

namespace ancient_defenders 
{
	enum class ETarget {
		Mage = 1,
		Golem = 2,
		Tower = 3,
		Spell = 4
    };

    struct RangeOfAttack {
        Float32 range;

        ETarget unitType;
        ETarget targetType;

        bool targetFound;
        Sequence<Entity> targets;
    };

    class RangedTargetingSystem
        : public System
    {
    public:
        inline String SystemName() { return "Ranged Targeting System"; }

        void Run() override;
    };
}