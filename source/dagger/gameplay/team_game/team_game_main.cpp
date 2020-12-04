#include "team_game_main.h"

#include <fstream>

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

<<<<<<< HEAD
#include "gameplay/team_game/game_manager.h"
#include "gameplay/team_game/team_game_player_input.h"
=======
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/character_controller.h"
>>>>>>> team/delta_snakes/main

using namespace dagger;

void team_game::TeamGame::GameplaySystemsSetup(Engine &engine_)
{
<<<<<<< HEAD
    engine_.AddSystem<GameManagerSystem>();
    engine_.AddSystem<TeamGamePlayerInputSystem>();
=======
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<CharacterControllerSystem>();
>>>>>>> team/delta_snakes/main
}

void team_game::TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

void team_game::SetupWorld(Engine& engine_)
{
    auto& reg = engine_.Registry();

    {

        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "logos:dagger");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 100 / ratio, 100  };

        auto& input = reg.emplace<InputReceiver>(entity);
        input.contexts.push_back("Controls");

        auto& character = reg.emplace<PlayerCharacter>(entity);
        character.speed = 50;
        ATTACH_TO_FSM(team_game::CharacterControllerFSM, entity);
    }
}
