#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct PingPongBall
{
    Vector3 speed{ 0, 0, 0 };
        
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
    void CreatePingPongBall(float tileSize_, Color color_, Vector3 speed_, Vector3 pos_);
};