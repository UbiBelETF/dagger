#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace PingPong
{
    struct ControllerMapping
    {
        DaggerKeyboard up_key;
        DaggerKeyboard down_key;

        Vector2 input{ 0, 0 };
    };

    class PingPongPlayerInputSystem
        : public System
    {
        static Float32 BOARDER_UP;
        static Float32 BOARDER_DOWN;

    public:

        static Float32 PLAYER_SPEED;

    public:
        inline String SystemName() { return "PingPong Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerOneInput(ControllerMapping& controllerMapping)
        {
            controllerMapping.up_key = DaggerKeyboard::KeyW;
            controllerMapping.down_key = DaggerKeyboard::KeyS;
        }

        static void SetupPlayerTwoInput(ControllerMapping& controllerMapping)
        {
            controllerMapping.up_key = DaggerKeyboard::KeyUp;
            controllerMapping.down_key = DaggerKeyboard::KeyDown;
        }

        static void SetupPlayerBoarders(Float32 boarderUp, Float32 boarderDown)
        {
            BOARDER_UP = boarderUp;
            BOARDER_DOWN = boarderDown;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent);
    };
}