#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ping_pong
{
    struct ControllerMapping
    {
        EDaggerKeyboard up_key;
        EDaggerKeyboard down_key;
        EDaggerKeyboard esc_key;//ADDED
        EDaggerKeyboard space_key;//ADDED
        bool boost;//ADDED
        bool exit;//ADDED
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
            controllerMapping_.up_key = EDaggerKeyboard::KeyW;
            controllerMapping_.down_key = EDaggerKeyboard::KeyS;
            controllerMapping_.esc_key = EDaggerKeyboard::KeyEscape;//ADDED
            controllerMapping_.space_key = EDaggerKeyboard::KeySpace;//ADDED
            controllerMapping_.boost = false;//ADDED
            controllerMapping_.exit = false;//ADDED
        }

        static void SetupPlayerTwoInput(ControllerMapping& controllerMapping_)
        {
            controllerMapping_.up_key = EDaggerKeyboard::KeyUp;
            controllerMapping_.down_key = EDaggerKeyboard::KeyDown;
            controllerMapping_.esc_key = EDaggerKeyboard::KeyBackspace;//ADDED
            controllerMapping_.space_key = EDaggerKeyboard::Key0;//ADDED
            controllerMapping_.boost = false;//ADDED
            controllerMapping_.exit = false;//ADDED
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