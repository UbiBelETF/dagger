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
        EDaggerKeyboard fireKey = EDaggerKeyboard::KeyJ;

        Vector2 input{ 0, 0 };
        Float32 inputFireLaser = 0;
    };

    struct RacingPlayerCar
    {
        Float32 horzSpeed;
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
