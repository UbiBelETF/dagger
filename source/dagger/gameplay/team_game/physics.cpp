#include "physics.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "gameplay/team_game/brawler_controller.h"




void team_game::PhysicsSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PhysicsSystem::OnEndOfFrame>(this);
}



void team_game::PhysicsSystem::Run()
{
    Engine::Registry().view< Physics>().each(
        [](Physics& physics_)
        {
            if (!physics_.Static) physics_.velocity.y += -10 * Engine::DeltaTime();
        });
}

void team_game::PhysicsSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PhysicsSystem::OnEndOfFrame>(this);
}

void team_game::PhysicsSystem::OnEndOfFrame()
{
    Engine::Registry().view<Physics, Transform ,BrawlerCharacter>().each(
        [](Physics& physics_, Transform& transform_ , BrawlerCharacter& char_)
        {
            transform_.position.x += physics_.velocity.x * Engine::DeltaTime();
            transform_.position.y += physics_.velocity.y * Engine::DeltaTime();
            physics_.velocity.x = 0;
            if (char_.run) char_.run = false;
        });
}


