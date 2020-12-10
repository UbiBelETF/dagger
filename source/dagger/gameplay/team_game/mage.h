#pragma once

#include "range_of_attack.h"

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
        Defending = 5,
    };

    struct MageStats {
        Float32 health;
        Float32 speed;
        Float32 meleeDmg;

        SInt32 postition = 0;
        Vector2 direction = { -1, 0 };
        Vector2 offset = { 0,0 };

        EAction currentAction = EAction::Moving;
    };

    struct Mage {
        Entity entity;
        Sprite & sprite;
        Transform & coordinates;
        Animator & animator;
        MageStats & mage;
        SimpleCollision & hitbox;
        RangeOfAttack & range;

        static Mage Get(Entity entity_);

        static Mage Create();
        
    };

    struct WalkingPath {
        static UInt32 numberOfPoints;
        static Sequence<Vector2> path;
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