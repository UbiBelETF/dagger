#include "gameplay/team_game/hero_controller.h"
#include "gameplay/team_game/team_game_main.h"

#include "core/game/transforms.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace team_game;

void TeamGameControllerSystem::Run()
{
    Engine::Registry().view<HeroControllerFSM::StateComponent>()
        .each([&](HeroControllerFSM::StateComponent& state_)
    {
        characterFSM.Run(state_);
    });
}
