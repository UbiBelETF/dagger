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
#include "gameplay/team_game/board.h"

#include <algorithm>
#include <random>

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<YSortingSystem>();
}

void MakeRandomGround(SInt32 x_, SInt32 y_)
{
    static Random::Sequence<String> grounds{
        "rock-0", "rock-1", "rock-2", "tree",
        "cross-0", "cross-1", "cross-2", "cross-3",
        "grave-0", "grave-1", "grave-2", "grave-3",
        "bones-0", "bones-1", "bones-2", "bones-3",
        "tiles-0", "tiles-1", "tiles-2", "tiles-3",
        "tiles-4", "tiles-5", "tiles-6", "tiles-7",
        "tiles-8"
    };

    auto& reg = Engine::Registry();

    auto tile = reg.create();
    auto& sprite = reg.emplace<Sprite>(tile);
    auto ground = grounds.Get();

    Bool hit = (ground.find("cross") == std::string::npos) 
        || (ground.find("grave") == std::string::npos) 
        || (ground.find("tree") == std::string::npos);

    AssignSprite(sprite, fmt::format("spritesheets:graveyard:{}", ground));
    sprite.position = Vector3{ x_ * 16, y_ * 16, 2000.0f };

    if (!hit)
    {
        sprite.color = { 1, 1, 1, 1 };
        Engine::GetDefaultResource<Board>()->space[x_][y_] = Occupacy::Neutral;
        auto& transform = reg.emplace<Transform>(tile);
    }
}

Entity MakePeep(String name_, Occupacy side_, SInt32 x_, SInt32 y_)
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
    sprite.position = Vector3{ x_ * 16, y_ * 16, 50.0f };
    sprite.scale.x = (side_ == Occupacy::Foe ? -1 : 1) * 1.1f;

    auto& anim = reg.emplace<Animator>(peep);
    AnimatorPlay(anim, fmt::format("tiny_heroes:{}-{}-side", name_, index), rand() % 3);

    return peep;
}

void TeamGame::WorldSetup(Engine& engine_)
{
    ShaderSystem::Use("standard");

    const SInt32 width = 40;
    const SInt32 height = 25;
    const Float32 chance = 0.82f;

    Engine::PutDefaultResource<Board>(new Board(width, height));

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 1280, 800 };
    camera->zoom = 4;
    camera->position = { (Float32)width / 2.0f * 16, (Float32)height / 2.0f * 16, 0 };
    camera->Update();

    for (SInt32 i = 0; i < width; i++)
    {
        for (SInt32 j = 0; j < height; j++)
        {
            if (Random::Uniform() > chance)
            {
                MakeRandomGround(i, j);
            }
        }
    }

    auto w4 = width / 4.0f;
    auto h2 = height / 2.0f;

    Entity a = MakePeep("human-knight", Occupacy::Friend, 15, 10);
    Entity b = MakePeep("human-warrior", Occupacy::Friend, 15, 12);
    Entity c = MakePeep("elf-mage", Occupacy::Friend, 16, 10);
    Entity d = MakePeep("human-mage", Occupacy::Friend, 16, 11);
    Engine::Registry().get<Sprite>(d).grayscale = 1.0f;

    Entity x = MakePeep("orc-knight", Occupacy::Foe, 25, 11);
    Entity y = MakePeep("orc-knight", Occupacy::Foe, 26, 11);
    Engine::Registry().get<Sprite>(y).grayscale = 1.0f;
    Entity z = MakePeep("orc-warrior", Occupacy::Foe, 25, 12);
    Engine::Registry().get<Sprite>(z).grayscale = 1.0f;
    Entity v = MakePeep("orc-assassin", Occupacy::Foe, 27, 14);
}
