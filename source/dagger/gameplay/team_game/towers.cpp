#include "towers.h"
#include "mage.h"

#include "core/engine.h"

using namespace ancient_defenders;

UInt32 ancient_defenders::TowerPlacementInfo::selectedSpot = 0;
Bool ancient_defenders::TowerPlacementInfo::spotLocked = false;
Sequence<Vector2> ancient_defenders::TowerPlacementInfo::spotCoordinates = {};

Float32 ancient_defenders::TowerStats::constructionGoal = 10.0f;

void ancient_defenders::TowerBehaviorSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&TowerBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::TowerBehaviorSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&TowerBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::TowerBehaviorSystem::Run()
{
    Engine::Registry().view<TowerStats, RangeOfAttack, Animator>().each(
        [](TowerStats & tower_, RangeOfAttack & range_, Animator & animator_)
    {
        if (tower_.constructionProgress < TowerStats::constructionGoal) return;
        else if (!tower_.constructed) {
            tower_.constructed = true;
            TowerPlacementInfo::spotLocked = false;

            auto & sprite = Engine::Registry().emplace<Sprite>(entt::to_entity(Engine::Registry(), tower_));
            AssignSprite(sprite, "ancient_defenders:" + tower_.type);
            sprite.position = { TowerPlacementInfo::spotCoordinates[tower_.address].x,TowerPlacementInfo::spotCoordinates[tower_.address].y, 1.0f };
            sprite.pivot = { 0,0.25f };
            sprite.scale = { 2,2 };
        }

        if (range_.targetFound) tower_.currentAction = EAction::Attacking;

        AnimatorPlay(animator_, "ancient_defenders:towers:" + tower_.type);
        if (tower_.currentAction == EAction::Idling) {}
        else if (tower_.currentAction == EAction::Attacking) {
            Engine::Registry().get<Health>(range_.target).currentHealth -= 2.0f*Engine::DeltaTime();
            tower_.currentAction = EAction::Idling;
        }
    });
}

void ancient_defenders::TowerBehaviorSystem::OnEndOfFrame()
{
}

Tower ancient_defenders::Tower::Get(Entity entity_)
{
    auto& reg = Engine::Registry();
    auto& pos = reg.get_or_emplace<Transform>(entity_);
    auto& anim = reg.get_or_emplace<Animator>(entity_);
    auto& tow = reg.get_or_emplace<TowerStats>(entity_);
    auto& col = reg.get_or_emplace<SimpleCollision>(entity_);
    auto& roa = reg.get_or_emplace<RangeOfAttack>(entity_);

    return Tower{ entity_, pos, anim, tow, col, roa };
}

Tower ancient_defenders::Tower::Create(String type_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto tow = Tower::Get(entity);

    tow.tower.type = type_;

    tow.coordinates.position = { TowerPlacementInfo::spotCoordinates[tow.tower.address].x,
            TowerPlacementInfo::spotCoordinates[tow.tower.address].y, 1.0f };

    tow.hitbox.size = { 250, 0 };
    tow.hitbox.shape = EHitbox::Circular;

    tow.range.range = tow.hitbox.size.x;

    tow.range.unitType = ETarget::Tower;
    tow.range.targetType = ETarget::Golem;

    return tow;
}