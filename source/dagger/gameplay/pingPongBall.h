#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct PingPongBall
{
    Vector3 speed{ 0, 0, 0 };
};

// todo: move to another file
class PingPongBallSystem
    : public System
{
    void Run() override;
};