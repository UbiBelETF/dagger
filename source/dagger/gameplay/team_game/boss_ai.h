#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace  dagger;
namespace team_game {
    struct BossAI
    {
        SInt32 run=0;
        Bool attack=false;
        Float32 delay = 0;
        
    };


    class BossAISystem: public System
    {
    public:
        inline String SystemName() { return "Boss System"; }
        void Run() override;

    };
}
