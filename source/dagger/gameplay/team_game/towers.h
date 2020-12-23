#pragma once

#include "core/core.h"
#include "core/system.h"

#include "mage.h"

using namespace dagger;

#define TOWER_NONE 101
#define TOWER_COUNT 6
#define SPOT_COUNT  8

namespace ancient_defenders {
    struct TowerMenuState
    {
        Entity towerMenuEntity;
        Entity towerMenuChoiceEntity;
        Entity towerSelectionEntity;
        Entity buildEntity;
        Entity buildImageEntity;
        Entity buildTimeTextEntity;
    };
    
    enum class SelectMode 
    {
        Spot = 1,
        Tower = 2,
    };

    struct TowerPlacementInfo 
    {
        static UInt32 selectedSpot;
        static String selectedTower;

        static SelectMode selectMode;

        static Sequence<Vector2> spotCoordinates;
        static StaticArray<Bool, SPOT_COUNT> availableSpot;
        static StaticArray<UInt32, SPOT_COUNT> chantingMages;
        static StaticArray<String, SPOT_COUNT> spotTowerNames;
        static StaticArray<String, TOWER_COUNT> towerNames;
    };

    struct TowerStats 
    {
        UInt32 address = TowerPlacementInfo::selectedSpot;

        Bool constructed = false;
        Float32 constructionProgress = 0.0f;
        static Float32 constructionGoal;

        EAction currentAction = EAction::Idling;
        String type;
    };


    struct Tower 
    {
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