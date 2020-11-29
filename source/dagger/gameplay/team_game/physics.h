#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game {
    class PhysicsSystem : public System
    {
        static Float32 ground ;
    public:
        String SystemName() override {
            return "Physic System";
        }
        static void SetGround(Float32 ground_);
        //void SpinUp() override;
        void Run() override;
        // void WindDown() override;
    };
}