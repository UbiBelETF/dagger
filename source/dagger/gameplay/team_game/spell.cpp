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
	Engine::Registry().view<SpellStats, RangeOfAttack, Sprite>().each(
		[](Entity entity_, SpellStats& spell_, RangeOfAttack& range_, Sprite& sprite_)
		{

            if (EPSILON_EQUAL(spell_.time, 1.0f)) {
                
                sprite_.scale = { 0.75f,0.75f };
                sprite_.pivot = { 0,0.2f };

            }
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

	spell.type = type_;
	coordinates.position = position_;
	spell.time = 1.0f;
	spell.dmg = 10.0f;
	AnimatorPlay(anim, "ancient_defenders:spell:" + spell.type);

	hitbox.shape = EHitbox::Circular;
	hitbox.size.x = 70.0f;

	range.unitType = ETarget::Spell;
	range.targetType = ETarget::Golem;

	return entity;
}
