#include "brawler_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"



#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/boss_ai.h"
#include "core/game/transforms.h"
using namespace team_game;


void BrawlerControllerSystem::Run()
{

    Engine::Registry().view<ControllerFSM::StateComponent>()
        .each([&](ControllerFSM::StateComponent& state_)
            {
                auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
                if (input_.Get("reset"))
                {
                    auto& transform = Engine::Registry().get<Transform>(state_.entity);
                    auto& brawler = Engine::Registry().get<BrawlerCharacter>(state_.entity);
                    transform.position={ 0,100 ,0};
                    brawler.healthHearts = 3;
                    auto view = Engine::Registry().view<BossAI, BrawlerCharacter, Transform>();
                    for (auto entity : view)
                    {
                            auto& transform2 = Engine::Registry().get<Transform>(entity);
                            auto& brawler = Engine::Registry().get<BrawlerCharacter>(entity);
                            transform2.position = { 2475,1125,0 };
                            brawler.healthHearts = 9;
                    }
                                              
                }
                m_ControllerFSM.Run(state_);
            });
    Engine::Registry().view<BossFSM::StateComponent>()
        .each([&](BossFSM::StateComponent& state_)
            {
                m_BossFSM.Run(state_);
            });

    
}