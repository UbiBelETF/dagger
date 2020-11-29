#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

#include "gameplay/plight/plight_states.h"

using namespace dagger;

namespace plight {
    struct StateRunning
    {
        int speed{ 100 };

        bool justEntered{ true };
        Float32 enterX { 0.f };
        Float32 enterY{ 0.f };
        bool changeState{ false };
        EPlightState nextState { EPlightState::running };
    };


    
    class PlightRunningStateSystem : public System
    {
        void OnInitialize(Registry& registry_, Entity entity_);
    public:
        inline String SystemName() { return "Plight Running State System"; }

        void Run() override;
        void SpinUp() override;
        void WindDown() override;
    private:
        void OnEndOfFrame();
    };
}