#include "spell.h"

#include "mage.h"
#include "range_of_attack.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"

#include "gameplay/common/simple_collisions.h"

#include <random>

using namespace ancient_defenders;


void ancient_defenders::SpellBehaviorSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&SpellBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::SpellBehaviorSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&SpellBehaviorSystem::OnEndOfFrame>(this);
}

void ancient_defenders::SpellBehaviorSystem::Run()
{
	Engine::Registry().view<SpellStats, Sprite, Transform, Animator, RangeOfAttack>().each(
		[](Entity entity_, SpellStats& spell_, Sprite& sprite_, Transform& transform_, Animator& animation_, RangeOfAttack& range_)
		{
			if (spell_.time > 0)
				spell_.time -= Engine::DeltaTime();
			else
				return;
			for (auto target : range_.targets) {
				Engine::Registry().get<Health>(target).currentHealth -= spell_.dmg * Engine::DeltaTime();
			}
				
		});

}

void ancient_defenders::SpellBehaviorSystem::OnEndOfFrame()
{
	
	auto view = Engine::Registry().view<SpellStats>();

	auto& reg = Engine::Registry();

	Sequence<Entity> toRemove{};
	auto it = view.begin();
	while (it != view.end()) {
		auto& en = view.get<SpellStats>(*it);
		if (en.time <= 0.0f) {
			toRemove.push_back(*it);
		}
		it++;
	}

	Engine::Registry().destroy(toRemove.begin(), toRemove.end());
}

Entity ancient_defenders::Spell::Create(Vector3 position_, String type_)
{
	auto& reg = Engine::Registry();
	auto entity = reg.create();
	auto& sprite = reg.emplace<Sprite>(entity);
	auto& coordinates = reg.emplace<Transform>(entity);
	auto& anim = reg.emplace<Animator>(entity);
	auto& spell = reg.emplace<SpellStats>(entity);
	auto& hitbox = reg.emplace<SimpleCollision>(entity);
	auto& range = reg.emplace<RangeOfAttack>(entity);

	//AssignSprite(sprite, "spritesheets:golem-little-sheet:golem_stand_side:1"); // Skipped because it was creating issues with HP sprite, Animator will add the sprite
 
	std::random_device dev;
	std::mt19937 rng(dev());

	std::uniform_int_distribution<std::mt19937::result_type> roll22(0, 22);
	std::uniform_int_distribution<std::mt19937::result_type> roll38(0, 38);

	std::uniform_int_distribution<std::mt19937::result_type> randomDirection(0, 1);

	spell.type = type_;
	coordinates.position = position_;
	spell.time = 1.0f;
	spell.dmg = 5.0f;
	AnimatorPlay(anim, "ancient_defenders:spell:" + spell.type);

	hitbox.size = sprite.size;
	range.range = hitbox.size.x;

	range.unitType = ETarget::Spell;
	range.targetType = ETarget::Golem;

	return entity;
}
