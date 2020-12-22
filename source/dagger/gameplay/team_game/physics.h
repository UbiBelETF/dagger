#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game {
    struct Physics {
        Vector2 velocity = { 0,0 };
        bool nonStatic = true;
        bool deathFloor = false;
    };
    static Float32 GetGravity() { return -500; }
    class PhysicsSystem : public System
    {
    public:
        String SystemName() override {
            return "Physic System";
        }
        void SpinUp() override;
        void Run() override;
        void WindDown() override;
        void OnEndOfFrame();
    };
}