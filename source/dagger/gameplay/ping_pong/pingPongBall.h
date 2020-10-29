#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace PingPong
{

    struct PingPongBall
    {
        Vector3 speed{ 0, 0, 0 };

        bool collided = false;
        Vector2 collisionSides{ 0, 0 };
    };

    class PingPongBallSystem
        : public System
    {
    public:

        inline String SystemName() { return "PingPong Ball System"; }

        void Run() override;
    };

}