#pragma once

#include "mage.h"

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

    enum class ESelectMode {
        Spot = 1,
        Tower = 2,
    };

    struct TowerPlacementInfo {
        static UInt32 selectedSpot;
        static String selectedTower;

        static ESelectMode selectMode;

        static Sequence<Vector2> spotCoordinates;
        static Sequence<Bool> availableSpot;
        static Sequence<String> towerNames;
    };

    struct TowerStats {
        UInt32 address = TowerPlacementInfo::selectedSpot;

        Bool constructed = false;
        Float32 constructionProgress = 0.0f;
        static Float32 constructionGoal;

        EAction currentAction = EAction::Idling;
        String type;
        Float32 cooldown = 0.0f;
        static Float32 maxCooldown;
    };


    struct Tower {
        Entity entity;

        static Entity Create(String type_);

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
