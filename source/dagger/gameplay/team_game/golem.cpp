#include "golem.h"

#include "mage.h"
#include "range_of_attack.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <random>

using namespace ancient_defenders;


void ancient_defenders::EnemyBehaviorSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&EnemyBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::EnemyBehaviorSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&EnemyBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::EnemyBehaviorSystem::Run()
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

				auto coords = WalkingPath::path[nextPosition];
				

				auto destinationX = coords.x + golem_.offset.x;
				auto destinationY = coords.y + golem_.offset.y;

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

				if (transform_.position.x == destinationX && transform_.position.y == destinationY) {
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

void ancient_defenders::EnemyBehaviorSystem::OnEndOfFrame()
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

	//AssignSprite(sprite, "spritesheets:golem-little-sheet:golem_stand_side:1"); // Skipped because it was creating issues with HP sprite, Animator will add the sprite
	
	auto start = WalkingPath::path.back();

    // Randomly create offset 
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> roll22(0, 22);
    std::uniform_int_distribution<std::mt19937::result_type> roll38(0, 38);

    std::uniform_int_distribution<std::mt19937::result_type> randomDirection(0, 1);

    // 22 is an important value because it represents the border at which character can move while still being on the path
    // 22 = 38 - 16( half of the width/height of the path - half of the character widht/height ); edge of the character sprite is alligned with the edge of the path
    // Only exception to this is when offset.y is positive at which point character can go up much higher while still appearing to walk along the path
    golem.offset = { (randomDirection(rng) ? roll22(rng) : 0.0f - roll22(rng)),(randomDirection(rng) ? roll38(rng) : 0.0f - roll22(rng)) };

    start.x += golem.offset.x;
    start.y += golem.offset.y;
 
    // Z axis is calculated this way to make bottom most character appear closest to the screen
    coordinates.position = { start.x, start.y, std::abs(golem.offset.y + 22.0f) };

    health.hpBar = reg.create();
	health.currentHealth = health.maxHealth = 100.0f;
	golem.meleeDmg = 5.0f;

	golem.speed = 50.0f;
	golem.direction = { -1,0 };

	hitbox.size = sprite.size;
	range.range = hitbox.size.x;

	range.unitType = ETarget::Golem;
	range.targetType = ETarget::Mage;

	return entity;
}
