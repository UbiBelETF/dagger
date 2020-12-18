#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace ancient_defenders {

    struct ControlData {
        Float32 timeout;
        static constexpr Float32 maxTimeout = 0.2f;
    };

    class PlayerControlsSystem
        : public System
    {
    public:
        inline String SystemName() { return "Player Controls System"; }

        void Run() override;
    
    private:
        ControlData m_controlData;
    };
}