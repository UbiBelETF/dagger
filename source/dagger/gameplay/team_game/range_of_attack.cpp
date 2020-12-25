#include "range_of_attack.h"

#include "core/engine.h"

#include "gameplay/common/simple_collisions.h"

void ancient_defenders::RangedTargetingSystem::Run() {

    Engine::Registry().view<SimpleCollision, RangeOfAttack>().each(
        [](SimpleCollision& collision_, RangeOfAttack& range_) 
		{
			range_.targets.clear();
			range_.targetFound = false;
			if (collision_.colided) {
				for (auto const& col : collision_.colisions) {
					if (!Engine::Registry().has<RangeOfAttack>(col)) continue;
					if (range_.targetType == Engine::Registry().get<RangeOfAttack>(col).unitType) {
						range_.targetFound = true;
						range_.targets.emplace_back(col);
					}
				}
			}
		});
}