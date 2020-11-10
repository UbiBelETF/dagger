#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{
    struct PingPongBall
    {
        Vector3 speed{ 0, 0, 0 };

        float changeSpeedPeriod{ 5.0f };
        float timeUnitlNextChange{ 5.0f };

        bool reachedGoal{ false };
        bool playerOneScored{ true };
        bool processed{ false };
    };

    struct PingPongWall
    {
        bool isLeft{ true };
    };

    class PingPongBallSystem
        : public System
    {
    public:

        inline String SystemName() { return "PingPong Ball System"; }

        void Run() override;
    };
}