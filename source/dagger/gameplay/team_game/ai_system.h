#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
using namespace dagger;
namespace team_game {
    class AiSystem : public System
    {
    private :
        float delay;
        float counter;
        int depth;
    public:
        AiSystem() { delay = 0.5f; counter = 0.0f; depth = 3; };
        void SpinUp() override;
        void Run() override;
        void WindDown() override;
        String SystemName() override {
            return "Team Game Ai System";
        }

    };
}
