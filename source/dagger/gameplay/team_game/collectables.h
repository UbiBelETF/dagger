#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    enum class CollectableType { HEALTH, SPEED, JUMP };

    struct Collectable
    {
        CollectableType type;

        Float32 improvement;
        Float32 powerUpTime{ 3.0f };
        Float32 timeLeft{ 3.0f };

        Bool pickedUp{ false };
        Bool removed{ false };
        Bool finished{ false };

        UInt8 userId;
    };

    class CollectablePickupSystem
        : public System
    {
        inline String SystemName() override {
            return "Collectable Pickup System";
        };

        void SpinUp() override;
        void WindDown() override;

        void Run() override;

    private:
        void OnEndOfFrame();
    };
}
