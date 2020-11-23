#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

    enum class EAction {
        Idling = 1,
        Attacking = 2,
        Moving = 3,
    };

    struct MageStats {
        Float32 health;
        Float32 speed;

        EAction currentAction = EAction::Idling;
        Float32 actionTime = 0; // Time spent either idling or moving

        static constexpr Float32 s_maxActionTime = 3.0f;
    };

    class MageBehaviorSystem
        : public System
    {

    public:
        inline String SystemName() { return "Racing Tools System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
    };

}