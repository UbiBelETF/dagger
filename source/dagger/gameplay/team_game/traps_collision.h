#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    class TrapsCollisionSystem
        : public System
    {
        StaticArray<Bool, 2> collided{ false, false };

        inline String SystemName() override {
            return "Traps collision System";
        };

        void Run() override;

        void SpinUp() override;
        void WindDown() override;

    private:
        void OnEndOfFrame();
    };
}
