#pragma once

#include "core/system.h"
#include "core/core.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"

using namespace dagger;

namespace ping_pong
{
    struct PingPongBall
    {
        Vector3 speed{ 0, 0, 0 };

        Float32 speedMultiplier{ 1.f };
        bool reachedGoal{ false };
        bool playerOneScored{ true };
        bool processed{ false };
        bool isMalicious{ false };
        bool toBeDestroyed{ false };
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

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
    };
}