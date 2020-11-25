#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"

using namespace dagger;

namespace plight {
    struct PlightCrosshair
    {
        Entity crosshairSprite;
    };

    class PlightAimingSystem : public System
    {
    public:
        inline String SystemName() { return "Plight Aiming System"; }

        void Run() override;
    };
}