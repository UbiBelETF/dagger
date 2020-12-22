#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

    struct PlayerInfo {
        Entity sprite;

        Float32 health = maxHealth;
        static constexpr Float32 maxHealth = 5000.0f;
    };

    class GameManagerSystem
        : public System
    {

    public:
        inline String SystemName() { return "Game Manager System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
    };

}