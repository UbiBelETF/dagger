#pragma once

#include "core/system.h"

using namespace dagger;

namespace ancient_defenders 
{
    enum class ETarget {
        Mage = 1,
        Golem = 2,
        Tower = 3,
    };

    struct RangeOfAttack {
        Float32 range;

        ETarget unitType;
        ETarget targetType;

        bool targetFound;
        Entity target;
    };

    class RangedTargetingSystem
        : public System
    {
    public:
        inline String SystemName() { return "Ranged Targeting System"; }

        void Run() override;
    };
}