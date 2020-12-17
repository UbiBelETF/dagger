#pragma once

#include "range_of_attack.h"
#include "hp_system.h"

#include "core/core.h"
#include "core/system.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

using namespace dagger;

namespace ancient_defenders {

    enum class EAction {
        Idling = 1,
        Moving = 2,
        Attacking = 3,
        Chanting = 4,
    };

    struct MageStats {
        Float32 speed;
        Float32 meleeDmg;

        UInt32 chantingSpot;

        SInt32 postition = 0;
        Vector2 direction = { -1, 0 };
        Vector2 offset = { 0,0 };

        EAction currentAction = EAction::Moving;
    };

    struct Mage {
        Entity entity;

        static Entity Create(Vector2 position_, EAction action_ = EAction::Idling, Bool offset_ = true);        
    };

    struct WalkingPath {
        static UInt32 numberOfPoints;
        static Sequence<Vector2> path;
    };


    class MageBehaviorSystem
        : public System
    {

    public:
        inline String SystemName() { return "Mage Behavior System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
    };

}