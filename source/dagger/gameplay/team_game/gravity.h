#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game {

    struct Gravity
    {
        Float32 terminalVelocity = 1000.f;
        Float32 verticalInitialSpeed{ 500.f };
        Float32 verticalCurrentSpeed{ 0.f };
    };

    class GravitySystem : public System
    {
    private:
        float m_GravityAcceleration = 1000.f;
    public:
        inline String SystemName() { return "Platformer Collisions System"; }

        void Run() override;
    };
}

