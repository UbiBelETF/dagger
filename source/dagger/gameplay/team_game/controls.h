#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ancient_defenders {

    class PlayerControlsSystem
        : public System
    {
    public:
        inline String SystemName() { return "Player Controls System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        Sequence<EDaggerKeyboard> m_pressedKeys = {};

        void OnKeyboardEvent(KeyboardEvent kEvent_);
        void OnEndOfFrame();
    };
}