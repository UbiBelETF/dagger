#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game {
    struct Physics {
        Vector2 velocity = { 0,0 };
        bool Static = false;
    };
    static Float32 gravity_acceleration = -500;
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