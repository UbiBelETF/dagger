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
#include "gameplay/team_game/game_manager.h"
using namespace team_game;


void BrawlerControllerSystem::Run()
{

    Engine::Registry().view<ControllerFSM::StateComponent>()
        .each([&](ControllerFSM::StateComponent& state_)
            {
                auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
                if (input_.Get("reset"))
                {
                    GameManagerSystem::completedObjective = true;
                    
                }
                m_ControllerFSM.Run(state_);
            });
    Engine::Registry().view<BossFSM::StateComponent>()
        .each([&](BossFSM::StateComponent& state_)
            {
                m_BossFSM.Run(state_);
            });

    
}