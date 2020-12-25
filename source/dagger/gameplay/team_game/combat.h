#pragma once
#include "core/core.h"
#include "core/system.h"
using namespace  dagger;
namespace team_game {
    class CombatSystem : public System
    {
    public:
        inline String SystemName() { return "Combat Collisions System"; }
        void Run() override;

    };
}