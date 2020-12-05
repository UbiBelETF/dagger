#include "brawler_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"



#include "gameplay/team_game/physics.h"
using namespace team_game;


void BrawlerControllerSystem::Run()
{
    Engine::Registry().view<ControllerFSM::StateComponent>()
        .each([&](ControllerFSM::StateComponent& state_)
            {
                FSMcontroller.Run(state_);
                auto& [astate_, physics_] = Engine::Registry().get<AnimationsFSM::StateComponent, Physics>(state_.entity);
                switch (state_.currentState)
                {
                case CharacterStates::Idle: FSManimator.GoTo(AnimationsState::Idle, astate_); break;
                case CharacterStates::Running: FSManimator.GoTo(AnimationsState::Running, astate_); break;
                case CharacterStates::InAir: {
                    if (physics_.velocity.y > 0) FSManimator.GoTo(AnimationsState::Jumping, astate_);
                    else  FSManimator.GoTo(AnimationsState::Falling, astate_);
                    break;
                }

                }
            });
    //THIS IS LEFT FOR THE SOLUTION IF WE HAVE SOMETHING IN RUN FUNCTIONS OF THE STATES
    /*Engine::Registry().view<AnimationsFSM::StateComponent>()
        .each([&](AnimationsFSM::StateComponent& state_)
            {
                FSManimator.Run(state_);
            });*/ 
}