#include "golem.h"

#include "mage.h"
#include "range_of_attack.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <math.h>
#include <list>

using namespace ancient_defenders;


void ancient_defenders::GolemBehaviorSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&GolemBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::GolemBehaviorSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&GolemBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::GolemBehaviorSystem::Run()
{
	Engine::Registry().view<Enemy, Sprite, Transform, Animator, RangeOfAttack>().each(
		[](Enemy& golem_, Sprite& sprite_, Transform& transform_, Animator& animation_, RangeOfAttack& range_)
		{
			if (range_.targetFound) golem_.currentAction = EAction::Attacking;

			if (golem_.currentAction == EAction::Idling) {
				AnimatorPlay(animation_, "ancient_defenders:golem:IDLE");
			}
			else if (golem_.currentAction == EAction::Moving) {
				auto nextPosition = golem_.postition - 1;

				auto coords = WalkingPath::path.back();
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
					golem_.direction.x = 1;
				}
				else if (transform_.position.x > destinationX) {
					golem_.direction.x = -1;
				}
				else golem_.direction.x = 0;
				if (transform_.position.y < destinationY) {
					golem_.direction.y = 1;
				}
				else if (transform_.position.y > destinationY) {
					golem_.direction.y = -1;
				}
				else golem_.direction.y = 0;

                if (golem_.direction.x != 0) {
                    sprite_.scale.x = golem_.direction.x * abs(sprite_.scale.x);
                }
				AnimatorPlay(animation_, "ancient_defenders:golem:WALK_SIDE");

				transform_.position.x += golem_.direction.x * golem_.speed * Engine::DeltaTime();
				transform_.position.y += golem_.direction.y * golem_.speed * Engine::DeltaTime();

				if ((transform_.position.x < destinationX && golem_.direction.x == -1) ||
					(transform_.position.x > destinationX && golem_.direction.x == 1)) {
					transform_.position.x = destinationX;
				}
				else if ((transform_.position.y > destinationY && golem_.direction.y == 1) ||
					(transform_.position.y < destinationY && golem_.direction.y == -1))
				{
					transform_.position.y = destinationY;
				}

				if (transform_.position.x == coords.x && transform_.position.y == coords.y) {
					golem_.postition--;
					if ((golem_.postition - 1) < 0) {
						golem_.currentAction = EAction::Idling;
						return;
					}
				}

			}
			else if (golem_.currentAction == EAction::Attacking) {
				AnimatorPlay(animation_, "ancient_defenders:golem:ATTACK_FRONT");
				Engine::Registry().get<Health>(range_.target).currentHealth -= golem_.meleeDmg* Engine::DeltaTime();
				golem_.currentAction = EAction::Moving; // Go back to moving after attacking
			}
		});

}

void ancient_defenders::GolemBehaviorSystem::OnEndOfFrame()
{
}

Entity ancient_defenders::Golem::Create()
{
	auto& reg = Engine::Registry();
	auto entity = reg.create();
	auto& sprite = reg.emplace<Sprite>(entity);
	auto& coordinates = reg.emplace<Transform>(entity);
	auto& anim = reg.emplace<Animator>(entity);
	auto& golem = reg.emplace<Enemy>(entity);
	auto& hitbox = reg.emplace<SimpleCollision>(entity);
	auto& range = reg.emplace<RangeOfAttack>(entity);
	auto& health = reg.emplace<Health>(entity);

    health.hpBar = reg.create();
    /*auto& hpBar = reg.emplace<Sprite>(health.hpBar);
    AssignSprite(hpBar, "spritesheets:hp-bar:hp_100");
    */

	AssignSprite(sprite, "spritesheets:golem-little-sheet:golem_stand_side:1");
	float ratio = sprite.size.y / sprite.size.x;
	//sprite.scale = { 2,2 };

	auto start = WalkingPath::path.back();

	coordinates.position = { start.x, start.y, 1.0f };

	health.currentHealth = health.maxHealth = 100.0f;
	golem.meleeDmg = 5.0f;

	golem.speed = 50.0f;
	golem.direction = { -1,0 };

	hitbox.size = sprite.size;
    hitbox.size.x += 10;
	range.range = hitbox.size.x;

	range.unitType = ETarget::Golem;
	range.targetType = ETarget::Mage;

	return entity;
}
