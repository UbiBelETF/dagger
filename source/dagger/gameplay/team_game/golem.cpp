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
				Engine::Registry().get<MageStats>(range_.target).health -= golem_.meleeDmg;
				golem_.currentAction = EAction::Moving; // Go back to moving after attacking
			}
		});

}

void ancient_defenders::GolemBehaviorSystem::OnEndOfFrame()
{

	auto view = Engine::Registry().view<MageStats>();

	auto it = view.begin();
	while (it != view.end()) {
		auto& en = view.get<MageStats>(*it);
		if (en.health <= 0.0f) {
			Engine::Registry().destroy(*it);
		}
		it++;
	}
}

Golem ancient_defenders::Golem::Get(Entity entity_)
{
	auto& reg = Engine::Registry();
	auto& sprite = reg.get_or_emplace<Sprite>(entity_);
	auto& pos = reg.get_or_emplace<Transform>(entity_);
	auto& anim = reg.get_or_emplace<Animator>(entity_);
	auto& gol = reg.get_or_emplace<Enemy>(entity_);
	auto& col = reg.get_or_emplace<SimpleCollision>(entity_);
	auto& roa = reg.get_or_emplace<RangeOfAttack>(entity_);

	return Golem{ entity_, sprite, pos, anim, gol, col, roa };
}

Golem ancient_defenders::Golem::Create()
{
	auto& reg = Engine::Registry();
	auto entity = reg.create();
	auto gol = Golem::Get(entity);

	AssignSprite(gol.sprite, "spritesheets:golem-little-sheet:golem_stand_side:1");
	float ratio = gol.sprite.size.y / gol.sprite.size.x;
	gol.sprite.scale = { 4,4 };

	auto start = WalkingPath::path.back();

	gol.coordinates.position = { start.x, start.y, 1.0f };

	gol.golem.health = 100.0f;
	gol.golem.meleeDmg = 0.2f;

	gol.golem.speed = 150.0f;
	gol.golem.direction = { -1,0 };

	gol.hitbox.size = gol.sprite.size;
	gol.range.range = gol.hitbox.size.x;

	gol.range.unitType = ETarget::Golem;
	gol.range.targetType = ETarget::Mage;

	return gol;
}
