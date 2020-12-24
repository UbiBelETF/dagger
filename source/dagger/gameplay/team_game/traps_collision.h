#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    struct Trap
    {
        Bool hadCollisionWithPlayer{ false };
        UInt8 collisionId{ 0 };
    };

    class TrapsCollisionSystem
        : public System
    {

        inline String SystemName() override {
            return "Traps collision System";
        };

        void Run() override;
    };
}
