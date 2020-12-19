#pragma once
#include "core/core.h"
#include "core/system.h"
using namespace  dagger;
namespace team_game {
    class CombatSystem : public System
    {
    public:
        inline String SystemName() { return "Collisions System"; }
        void Run() override;

    };
}