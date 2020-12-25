#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
using namespace dagger;
namespace team_game {
    struct Health {
        INT32 hp;
        INT32 maxHp;
        bool show = true;
        Entity hpBar;
    };
    class HealthSystem : public System
    {


    public:
        void Run() override;
        String SystemName() override {
            return "Team Game Health System";
        }



    };

}