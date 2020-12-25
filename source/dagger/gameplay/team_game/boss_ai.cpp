#include "boss_ai.h"

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

using namespace std;
void team_game::BossAISystem::Run()
{
    auto viewCollisions = Engine::Registry().view<BrawlerCharacter, SimpleCollision, Transform>();
    auto view = Engine::Registry().view< BrawlerCharacter, BossAI, Transform,Sprite>();
    for (auto entity1 : view)
    {
        auto& bossCharacter = Engine::Registry().get<BrawlerCharacter>(entity1);
        
            for (auto entity2 : viewCollisions)
            {
                if (entity2 != entity1)
                {
                    auto& enemyTransform = Engine::Registry().get<Transform>(entity2);
                    auto& bossTransform = Engine::Registry().get<Transform>(entity1);
                    auto& bossAI = Engine::Registry().get<BossAI>(entity1);
                    auto& sprite = Engine::Registry().get<Sprite>(entity1);
                    if (enemyTransform.position.x < bossTransform.position.x - bossCharacter.attackSize/2 && std::abs(bossTransform.position.y - enemyTransform.position.y) <50)
                    {
                        bossAI.run = -1;
                    }
                    else if (enemyTransform.position.x > bossTransform.position.x + bossCharacter.attackSize/2 && std::abs(bossTransform.position.y - enemyTransform.position.y) <50 )
                    {
                        bossAI.run = 1;
                    }
                    else if (std::abs(bossTransform.position.y - enemyTransform.position.y) <32 && std::abs(bossTransform.position.y - enemyTransform.position.y)>30 && bossAI.delay==0)
                    {
                        if (enemyTransform.position.x < bossTransform.position.x) sprite.scale.x = -1;
                        else sprite.scale.x = 1;
                        bossAI.attack = true;
                        bossAI.delay = 1.5;
                    }
                    if (bossAI.delay > 0)
                    {
                        bossAI.delay -= Engine::DeltaTime();
                        if (bossAI.delay <= 0) bossAI.delay = 0;
                    }
                }
            }
        

    }

}
