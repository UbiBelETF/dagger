#pragma once

#include "core/system.h"

using namespace dagger;

namespace ancient_defenders 
{
    struct RangeOfAttack;

    class RangedTargetingSystem
        : public System
    {
    public:
        inline String SystemName() { return "Ranged Targeting System"; }

        void Run() override;
    };
}