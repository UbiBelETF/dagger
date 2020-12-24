#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{

    struct TreasureChest
    {
        Bool hadCollisionWithPlayer{ false };
        UInt8 playerId{ 0 };
    };

    class TreasureSystem
        : public System
    {
        inline String SystemName() override {
            return "Treasure System";
        };

        void Run() override;
    };
}
