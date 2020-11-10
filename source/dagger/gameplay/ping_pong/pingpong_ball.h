#pragma once

#include "core/system.h"
#include "core/core.h"
#include "core/engine.h"

#include <chrono>

using namespace dagger;

namespace ping_pong
{
    struct PingPongBall
    {
        Vector3 speed{ 0, 0, 0 };
        
        std::chrono::seconds changeSpeedPeriod{ 5 };
        TimePoint lastSpeedChange{ dagger::Engine::Instance().CurrentTime() };

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