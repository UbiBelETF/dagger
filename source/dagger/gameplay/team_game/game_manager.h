#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

    struct PlayerInfo {
        Entity hpSprite;
        Entity countdownTimer;

        Float32 health = maxHealth;
        static constexpr Float32 maxHealth = 3000.0f;

        Float32 spawnTimer = 0.0f;
        Float32 timeLeft = 120.0f;
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
        Bool m_victory = false;
        Bool m_defeat = false;

        
        void OnEndOfFrame();
    };

}