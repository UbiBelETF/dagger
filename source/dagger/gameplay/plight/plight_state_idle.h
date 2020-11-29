#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

#include "gameplay/plight/plight_states.h"

using namespace dagger;

namespace plight {
    struct StateIdle
    {
        bool changeState{ false };
        bool resting{ false };

        Float32 restingTime = 0.75f;
        Float32 currentRestingTime = 0.f;

        EPlightState nextState{ EPlightState::idle };
        Float32 nextRunX{ 0.f };
        Float32 nextRunY{ 0.f };
    };

   

    class PlightIdleStateSystem : public System
    {
        void OnInitialize(Registry& registry_, Entity entity_);
    public:
        inline String SystemName() { return "Plight Idle State System"; }

        void Run() override;
        void SpinUp() override;
        void WindDown() override;
    private:
        void OnEndOfFrame();
    };
}