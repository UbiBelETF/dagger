#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ancient_defenders {

    struct ControlData {
        static Float32 timeout;
        static Float32 maxTimeout;
    };

    class PlayerControlsSystem
        : public System
    {
    public:
        inline String SystemName() { return "Player Controls System"; }

        void Run() override;
    };
}