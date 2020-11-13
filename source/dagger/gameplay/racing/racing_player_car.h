#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace racing_game
{
    struct ControllerMapping
    {
        EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
        EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;

        Vector2 input{ 0, 0 };
    };

    struct RacingPlayerCar
    {
        Float32 horzSpeed;
        int strike;
        int pause=10;
    };

    class RacingPlayerInputSystem
        : public System
    {
    public:
        inline String SystemName() { return "Racing Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}
