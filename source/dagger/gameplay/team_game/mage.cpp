#include "mage.h"
#include "range_of_attack.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <math.h>
#include <iostream>
#include <list>

using namespace ancient_defenders;

// Initialize static members
UInt32 ancient_defenders::WalkingPath::numberOfPoints = 0;
std::list<Vector2> ancient_defenders::WalkingPath::path = {};

void ancient_defenders::MageBehaviorSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&MageBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::MageBehaviorSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&MageBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::MageBehaviorSystem::Run()
{
    Engine::Registry().view<MageStats, Sprite, Transform, Animator, RangeOfAttack>().each(
        [](MageStats& mage_, Sprite& sprite_, Transform& transform_, Animator& animation_, RangeOfAttack& range_)
    {
        if (range_.targetFound) mage_.currentAction = EAction::Attacking;

        if (mage_.currentAction == EAction::Idling) {
            AnimatorPlay(animation_, "ancient_defenders:mage:IDLE");
        }
        else if (mage_.currentAction == EAction::Moving) {
            auto nextPosition = mage_.postition + 1;

            auto coords = WalkingPath::path.front();
            int i = 0;
            for (auto iter : WalkingPath::path) {
                if (i == nextPosition) {
                    coords = iter;
                    break;
                }
                i++;
            }

            auto destinationX = coords.x; // Maybe add +/- a few percent to make paths bit more varied
            auto destinationY = coords.y;

            if (transform_.position.x < destinationX) {
                mage_.direction.x = 1;
            }
            else if (transform_.position.x > destinationX) {
                mage_.direction.x = -1;
            }
            else mage_.direction.x = 0;
            if (transform_.position.y < destinationY) {
                mage_.direction.y = 1;
            }
            else if (transform_.position.y > destinationY) {
                mage_.direction.y = -1;
            }
            else mage_.direction.y = 0;

            if (mage_.direction.x != 0) {
                sprite_.scale.x = (-mage_.direction.x) * abs(sprite_.scale.x);
                AnimatorPlay(animation_, "ancient_defenders:mage:WALK_SIDE");
            }
            else if (mage_.direction.y == 1) {
                AnimatorPlay(animation_, "ancient_defenders:mage:WALK_UP");
            }
            else {
                AnimatorPlay(animation_, "ancient_defenders:mage:WALK_DOWN");
            }

            transform_.position.x += mage_.direction.x * mage_.speed * Engine::DeltaTime();
            transform_.position.y += mage_.direction.y * mage_.speed * Engine::DeltaTime();

            if ((transform_.position.x < destinationX && mage_.direction.x == -1) ||
                (transform_.position.x > destinationX && mage_.direction.x == 1)) {
                transform_.position.x = destinationX;
            }
            else if ((transform_.position.y > destinationY && mage_.direction.y == 1) ||
                (transform_.position.y < destinationY && mage_.direction.y == -1)) 
            {
                transform_.position.y = destinationY;
            }

            if (transform_.position.x == coords.x && transform_.position.y == coords.y) {
                mage_.postition++;
                if ((mage_.postition + 1) >= WalkingPath::numberOfPoints) {
                    mage_.currentAction = EAction::Idling;
                    return;
                }
            }

        }
        else if (mage_.currentAction == EAction::Attacking) {
            if (mage_.direction.x != 0) {
                sprite_.scale.x = (-mage_.direction.x) * abs(sprite_.scale.x);
                AnimatorPlay(animation_, "ancient_defenders:mage:ATTACK_SIDE");
            }
            else if (mage_.direction.y == 1) {
                AnimatorPlay(animation_, "ancient_defenders:mage:ATTACK_BACK");
            }
            else {
                AnimatorPlay(animation_, "ancient_defenders:mage:ATTACK_FRONT");
            }
            //Engine::Registry().get<Enemy>(range_.target).health -= mage_.meleeDmg;
        }
        else if (mage_.currentAction == EAction::Chanting) {
            // Increase tower building progres
        }
        else if (mage_.currentAction == EAction::Defending) {
            // Cast spells
        }
    });
  
}

void ancient_defenders::MageBehaviorSystem::OnEndOfFrame()
{
}

Mage ancient_defenders::Mage::Get(Entity entity_)
{
    auto& reg = Engine::Registry();
    auto& sprite = reg.get_or_emplace<Sprite>(entity_);
    auto& pos = reg.get_or_emplace<Transform>(entity_);
    auto& anim = reg.get_or_emplace<Animator>(entity_);
    auto& mag = reg.get_or_emplace<MageStats>(entity_);
    auto& col = reg.get_or_emplace<SimpleCollision>(entity_);
    auto& roa = reg.get_or_emplace<RangeOfAttack>(entity_);

    return Mage{ entity_, sprite, pos, anim, mag, col, roa };
}

Mage ancient_defenders::Mage::Create()
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    auto mag = Mage::Get(entity);

    AssignSprite(mag.sprite, "spritesheets:mage:mage_stand_side:1");
    float ratio = mag.sprite.size.y / mag.sprite.size.x;
    mag.sprite.scale = { 4,4 };

    auto start = WalkingPath::path.front();
    
    mag.coordinates.position = { start.x, start.y, 1.0f };

    mag.mage.speed = 150.0f;
    mag.mage.direction = { -1,0 };

    mag.hitbox.size = mag.sprite.size;
    mag.range.range = mag.hitbox.size.x;

    mag.range.unitType = ETarget::Mage;
    mag.range.targetType = ETarget::Golem;

    return mag;
}
