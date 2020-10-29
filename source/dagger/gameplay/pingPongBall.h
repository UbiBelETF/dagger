#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct ControllerMapping
{
    DaggerKeyboard key;
    Vector2 input;
};

struct PingPongBall
{
    Vector3 speed{ 0, 0, 0 };

    bool collided = false;
    Vector2 collisionSides{ 0, 0 };
};

// todo: move to another file
class PingPongBallSystem
    : public System
{
    Vector2 m_input;

public:

    inline String SystemName() { return "Ping Pong Ball System"; }

    void SpinUp() override;
    void WindDown() override;
    void Run() override;

private:

    void OnKeyboardEvent(KeyboardEvent kEvent);
};