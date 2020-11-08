#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{
    class PingPongBallRandomSpeedSystem
        : public System
    {
        static constexpr Float32 TIME_TO_CHANGE_SPEED = 2.f;
        Float32 m_timer = 0;

    public:

        inline String SystemName() { return "PingPong Ball Random Speed Change System"; }

        void Run() override;
    };
}