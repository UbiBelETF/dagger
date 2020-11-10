#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace racing_game
{
    struct BigLaserGun
    {
        EDaggerKeyboard fireKey = EDaggerKeyboard::KeyJ;

        Float32 remainingCooldown = 0, laserSpeed;
    };

    class BigLaserSystem
        : public System
    {
    public:
        inline String SystemName() { return "Big Laser System"; }

        void Run() override;
    };
}
