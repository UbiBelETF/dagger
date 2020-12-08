#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game {

    struct Gravity
    {
        Float32 airTime = 0.f;
        Float32 terminalVelocity = 5.f;
    };

    class GravitySystem : public System
    {
    private:
        float m_GravityAcceleration = 0.5f;
    public:
        inline String SystemName() { return "Platformer Collisions System"; }

        void Run() override;
    };
}

