#include "range_of_attack.h"

#include "core/engine.h"

#include "gameplay/common/simple_collisions.h"

#include <list>

struct Enemy;

void ancient_defenders::RangedTargetingSystem::Run() {

    Engine::Registry().view<SimpleCollision, RangeOfAttack>().each(
        [](SimpleCollision& collision, RangeOfAttack& range) 
    {
        std::for_each(collision.colisions.begin(), collision.colisions.end(),
            [](const entt::entity col) 
        {
            if (Engine::Registry().view<Enemy>.has(col)) {
                // Signal the mage this ROA is tied to, if mage isn't attacking already
            }
        });
    
    });
}