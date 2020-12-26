#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "health_system.h"
#include "core/game/transforms.h"
using namespace dagger;
namespace team_game {
    struct Attack {
        UInt32 damage;
        std::vector<Entity>damaged;
        bool finished = false;
        Entity  attackEnt;
        Vector3 offsetVec;
    };

    struct AttackOffset {
        
    };
    class AttackSystem : public System
    {
    public:
      
        void Run() override;
        String SystemName() override {
            return "Team Game Attack System";
        }

    };
}
