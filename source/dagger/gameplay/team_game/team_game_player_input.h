#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace team_game
{
    class TeamGamePlayerInputSystem : public System
    {
    public:

        Bool triggeredTransition{ false };
        EDaggerKeyboard nextLevelKey{ EDaggerKeyboard::KeyP };

        inline String SystemName() { return "Player Input System "; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnKeyboardEvent(KeyboardEvent kEvent_);
    };
};
