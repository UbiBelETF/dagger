#include "team_game_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/ysort.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/random.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<YSortingSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

void MakePeep(String name_)
{
    static std::unordered_map<String, UInt32> amounts{
        { "human-warrior", 3 },
        { "human-knight", 18 },
        { "human-bard", 2 },
        { "human-peasant", 6 },
        { "human-archer", 1 },
        { "orc-warrior", 2 },
        { "human-priest", 5 },
        { "orc-assassin", 2 },
        { "orc-knight", 4 },
        { "human-mage", 2 },
        { "human-assassin", 1 },
        { "skeleton-warrior", 5 },
        { "skeleton-mage", 4 },
        { "skeleton-priest", 3 },
        { "skeleton-knight", 7 },
        { "undead-warrior", 4 },
        { "elf-warrior", 3 },
        { "demon-warrior", 5 },
        { "elf-knight", 3 },
        { "boar-warrior", 1 },
        { "elf-assassin", 3 },
        { "demon-knight", 2 },
        { "elf-mage", 3 },
    };

    auto& reg = Engine::Registry();

    int index = rand() % amounts[name_];

    auto peep = reg.create();
    auto& sprite = reg.emplace<Sprite>(peep);
    reg.emplace<YSort>(peep);
    AssignSprite(sprite, fmt::format("spritesheets:tiny_heroes:{}-{}-side:1", name_, index));
    sprite.position = Vector3{ Random::InEllipse(30.0f * Vector2{1, 0.5f}), 0.0f };

    auto& anim = reg.emplace<Animator>(peep);
    AnimatorPlay(anim, fmt::format("tiny_heroes:{}-{}-side", name_, index), rand() % 3);
}

void team_game::SetupWorld(Engine &engine_)
{
    for (int i = 0; i < 10; i++)
    {
        MakePeep("human-peasant");
    }
}
