#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    class EnemyPatrolSystem : public System
    {
    public:
        inline String SystemName() override {
            return "Enemy Patrol System";
        };

        void Run() override;

    };
}
