#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

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
    
    struct TowerPlacementInfo 
    {
        UInt32 selectedSpot;
        String selectedTower;

        Sequence<Vector2> spotCoordinates;
        StaticArray<Bool, SPOT_COUNT> availableSpot;
        StaticArray<UInt32, SPOT_COUNT> chantingMages;
        StaticArray<String, SPOT_COUNT> spotTowerNames;
        StaticArray<String, TOWER_COUNT> towerNames;
    };

    struct TowerStats 
    {
        UInt32 address = Engine::GetDefaultResource<TowerPlacementInfo>()->selectedSpot;

        Bool constructed = false;
        Float32 constructionProgress = 0.0f;
        static Float32 constructionGoal;

        EAction currentAction = EAction::Idling;
        String type;
        Float32 cooldown = 0.0f;
        static Float32 maxCooldown;
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