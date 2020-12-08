#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

using namespace dagger;

namespace plight {
    struct PlightCrosshair
    {
        Entity crosshairSprite;
        Float32 angle { 0.f };
        Float32 playerDistance{ 20.f };

        Float32 rotationSpeed{ 5.f };
    };

    class PlightAimingSystem : public System
    {
        
    public:
        inline String SystemName() { return "Plight Aiming System"; }

        void Run() override;
    };
}