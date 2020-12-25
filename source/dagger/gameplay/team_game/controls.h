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

        void Run() override;
    
    private:
        static Bool s_onStart;
    };
}