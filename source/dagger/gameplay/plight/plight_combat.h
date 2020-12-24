#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"


#define STAMINA_FOR_RUNNING_FRAME 0.f
#define STAMINA_FOR_REGENERATING_FRAME 0.2f
#define STAMINA_FOR_DASHING_FRAME 1.f
#define BAR_START_SIZE 50.f

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
        Float32 healthBarOffset{0.f};

        Entity backgroundStaminaBar;
        Entity currentStaminaBar;
        Float32 staminaBarOffset{0.f};

        Float32 playerDistance{ 40.f };
        
        Float32 updateTimer{ 0.01f };
        Float32 currentTimer{ 0.f };
    };

    class PlightCombatSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Combat System"; }

        void Run() override;
        void WindDown() override;
        void SpinUp() override;
    private:
        void OnEndOfFrame();

        Float32 getDistance(Float32 x1, Float32 y1, Float32 x2, Float32 y2);
    };
}