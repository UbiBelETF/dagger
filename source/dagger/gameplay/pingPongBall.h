#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct PingPongBall
{
    Vector3 speed{ 0, 0, 0 };

    bool collided = false;
    Vector3 pointOfCollision{ 0, 0, 0 };
};

// todo: move to another file
class PingPongBallSystem
    : public System
{
    inline String SystemName() { return "Ping Pong Ball System"; }

    void Run() override;
};