#include "towers.h"
#include "mage.h"
#include "spell.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace ancient_defenders;

UInt32 ancient_defenders::TowerPlacementInfo::selectedSpot = TOWER_NONE;
String ancient_defenders::TowerPlacementInfo::selectedTower = "BLOOD";
SelectMode ancient_defenders::TowerPlacementInfo::selectMode = SelectMode::Tower;

Sequence<Vector2> ancient_defenders::TowerPlacementInfo::spotCoordinates = {};
StaticArray<Bool, SPOT_COUNT> ancient_defenders::TowerPlacementInfo::availableSpot = { true, true, true, true, true, true, true, true };
StaticArray<UInt32, SPOT_COUNT> ancient_defenders::TowerPlacementInfo::chantingMages = { 0, 0, 0, 0, 0, 0, 0, 0 };
StaticArray<String, SPOT_COUNT> ancient_defenders::TowerPlacementInfo::spotTowerNames = { "", "", "", "", "", "", "", "" };
StaticArray<String, TOWER_COUNT> ancient_defenders::TowerPlacementInfo::towerNames = {"BLOOD", "FIRE", "ICE", "POISON", "STORM", "SUN"};

Float32 ancient_defenders::TowerStats::constructionGoal = 10.0f;
Float32 ancient_defenders::TowerStats::maxCooldown = 2.0f;


void ancient_defenders::TowerBehaviorSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&TowerBehaviorSystem::OnEndOfFrame>(this);
    Engine::PutDefaultResource<TowerMenuState>(new TowerMenuState{});
}

void ancient_defenders::TowerBehaviorSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&TowerBehaviorSystem::OnEndOfFrame>(this);
    delete Engine::GetDefaultResource<TowerMenuState>();
}

void ancient_defenders::TowerBehaviorSystem::Run()
{
    Engine::Registry().view<TowerStats, RangeOfAttack, Animator>().each(
        [](Entity entity_, TowerStats & tower_, RangeOfAttack & range_, Animator & animator_)
    {
        if (tower_.constructionProgress < TowerStats::constructionGoal) return;
        else if (!tower_.constructed) {
            tower_.constructed = true;
            TowerPlacementInfo::chantingMages[tower_.address] = 0;
            TowerPlacementInfo::availableSpot[tower_.address] = false;


            auto & sprite = Engine::Registry().emplace<Sprite>(entity_);
            AssignSprite(sprite, "ancient_defenders:" + tower_.type);
            sprite.position = { TowerPlacementInfo::spotCoordinates[tower_.address].x,TowerPlacementInfo::spotCoordinates[tower_.address].y, 1.0f };
            sprite.pivot = { 0,0.25f };
            sprite.scale = { 2,2 };
        }

        if ((tower_.cooldown -= Engine::DeltaTime()) > 0) return;
        
        if (range_.targetFound) tower_.currentAction = EAction::Attacking;
        
        AnimatorPlay(animator_, "ancient_defenders:towers:" + tower_.type);
        if (tower_.currentAction == EAction::Idling) {}
        else if (tower_.currentAction == EAction::Attacking) {
            Spell::Create(Engine::Registry().get<Transform>(range_.targets[0]).position, tower_.type);
            tower_.currentAction = EAction::Idling;
            tower_.cooldown = TowerStats::maxCooldown;

        }
    });
}

void ancient_defenders::TowerBehaviorSystem::OnEndOfFrame()
{
}


Entity ancient_defenders::Tower::Create(String type_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& coordinates = reg.emplace<Transform>(entity);
    auto& anim = reg.emplace<Animator>(entity);
    auto& tower = reg.emplace<TowerStats>(entity);
    auto& hitbox = reg.emplace<SimpleCollision>(entity);
    auto& range = reg.emplace<RangeOfAttack>(entity);

    tower.type = type_;

    coordinates.position = { TowerPlacementInfo::spotCoordinates[tower.address].x,
            TowerPlacementInfo::spotCoordinates[tower.address].y, 1.0f };

    hitbox.size = { 250, 0 };
    hitbox.shape = EHitbox::Circular;

    range.range = hitbox.size.x;

    range.unitType = ETarget::Tower;
    range.targetType = ETarget::Golem;

    return entity;
}