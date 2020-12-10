#pragma once

#include "mage.h"

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

    struct TowerPlacementInfo {
        static UInt32 selectedSpot;
        static Bool spotLocked;
        static Sequence<Vector2> spotCoordinates;
    };

    struct TowerStats {
        UInt32 address = TowerPlacementInfo::selectedSpot;

        Bool constructed = false;
        Float32 constructionProgress = 0.0f;
        static Float32 constructionGoal;

        EAction currentAction = EAction::Idling;
        String type;
    };


    struct Tower {
        Entity entity;
        Transform & coordinates;
        Animator & animator;
        TowerStats & tower;
        SimpleCollision & hitbox;
        RangeOfAttack & range;

        static Tower Get(Entity entity_);

        static Tower Create(String type_);

    };


    class TowerBehaviorSystem
        : public System
    {
    public:
        inline String SystemName() { return "Tower Behavior System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:

        void OnEndOfFrame();
    };
}
