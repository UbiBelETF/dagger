#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace pingPong
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
        static Float32 s_BoarderUp;
        static Float32 s_BoarderDown;

    public:

        static Float32 s_PlayerSpeed;

    public:
        inline String SystemName() { return "PingPong Player Input System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        static void SetupPlayerOneInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = DaggerKeyboard::KeyW;
            controllerMapping_.down_key = DaggerKeyboard::KeyS;
        }

        static void SetupPlayerTwoInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = DaggerKeyboard::KeyUp;
            controllerMapping_.down_key = DaggerKeyboard::KeyDown;
        }

        static void SetupPlayerBoarders(Float32 boarderUp_, Float32 boarderDown_)
        {
            s_BoarderUp = boarderUp_;
            s_BoarderDown = boarderDown_;
        }

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
}