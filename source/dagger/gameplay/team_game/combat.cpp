#include "combat.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/collisions.h"
#include <algorithm>
#include <execution>

void team_game::CombatSystem::Run()
{
    auto viewCollisions = Engine::Registry().view<BrawlerCharacter, SimpleCollision,Transform>();
    auto view = Engine::Registry().view< BrawlerCharacter, SimpleCollision,Transform,Sprite>();
    for (auto entity : view)
    {
        auto& brawler_ = Engine::Registry().get<BrawlerCharacter>(entity);
        auto& col_ = Engine::Registry().get<SimpleCollision>(entity);
        auto& t_ = Engine::Registry().get<Transform>(entity);
        auto& sprite_ = Engine::Registry().get<Sprite>(entity);
        if (EPSILON_NOT_ZERO(brawler_.attacking))
        {
            Float32 proc_ = (int)(brawler_.attacking * 100);
            Vector3 temp_t = { 0,0,0 };
            temp_t.x = t_.position.x;
            temp_t.x+=(brawler_.attackSize/2 * sprite_.scale.x);
            temp_t.y = t_.position.y;
            temp_t.z = t_.position.z;
            col_.size.x += brawler_.attackSize / (2 * brawler_.hitSize);
            for (auto entity2 : viewCollisions)
            {
                if (entity2 != entity)
                {
                    BrawlerCharacter& braw_ = Engine::Registry().get<BrawlerCharacter>(entity2);
                    SimpleCollision& collision = viewCollisions.get<SimpleCollision>(entity2);
                    Transform& transform = viewCollisions.get<Transform>(entity2);
                    if (col_.IsCollided(temp_t, collision, transform.position) && !brawler_.hittedEnemy)
                    {
                        braw_.healthHearts--;
                        brawler_.hittedEnemy = true;
                        braw_.gotHit = 0.2f;
                        if (braw_.healthHearts == 0);
                    }
                }
            }
            col_.size.x -= brawler_.attackSize/(2*brawler_.hitSize);
           
        }
    }
}
