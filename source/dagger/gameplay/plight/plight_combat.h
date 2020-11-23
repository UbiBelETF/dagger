#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"


using namespace dagger;

namespace plight {
    struct CombatStats
    {
        Float32 maxHealth{ 100 };
        Float32 currentHealth{ 100 };

        Float32 maxStamina{ 100 };
        Float32 currentStamina{ 100 };

        Entity backgroundHealthBar;
        Entity currentHealthBar;

        Entity backgroundStaminaBar;
        Entity currentStaminaBar;
    };

    class PlightCombatSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Combat System"; }

        void Run() override;
    };
}