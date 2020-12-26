#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "health_system.h"
#include "core/game/transforms.h"
using namespace dagger;
namespace team_game {
    class AttackSystem : public System
    {
    public:
        struct Attack {
            UInt32 damage;
            std::vector<Health>damaged;
            bool finished=true;
            Entity  orig;
        };

        struct AttackOffset {
            Vector3 offsetVec;
        };
        void Run() override;
        String SystemName() override {
            return "Team Game Attack System";
        }

    };
}
