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
        Engine::GetDefaultResource<Board>()->space[x_][y_] = (UInt32)Occupacy::Neutral;
    }
}

Peep MakePeep(String name_, Occupacy side_, SInt32 x_, SInt32 y_)
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

    Engine::GetDefaultResource<Board>()->space[x_][y_] = (UInt32)side_;

    return Peep{ peep, side_ };
}

PeepGroup MakeGroup(String name_, Occupacy side_, std::initializer_list<Peep> peeps_)
{
    PeepGroup group;
    group.name = name_;
    group.side = side_;

    for (auto peep : peeps_) group.peeps.push_back(peep);

    return group;
}

PeepArmy MakeArmy(String name_, Occupacy side_, std::initializer_list<PeepGroup> groups_)
{
    PeepArmy army;
    army.name = name_;
    army.side = side_;
    
    for (auto group : groups_) army.groups.push_back(group);

    return army;
}

void TeamGame::WorldSetup(Engine& engine_)
{
    ShaderSystem::Use("standard");

    const SInt32 width = 40;
    const SInt32 height = 25;
    const Float32 chance = 0.82f;

    Engine::PutDefaultResource<Board>(new Board(width, height));
    auto* board = Engine::GetDefaultResource<Board>();

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 1280, 800 };
    camera->zoom = 2;
    camera->position = { (Float32)width / 2.0f * 16, (Float32)height / 2.0f * 16, 0 };
    camera->Update();

    auto a = MakePeep("human-knight", Occupacy::Friend, 15, 15);
    auto b = MakePeep("human-knight", Occupacy::Friend, 15, 16);
    auto knights = MakeGroup("Knights", Occupacy::Friend, { a, b });

    auto c = MakePeep("elf-mage", Occupacy::Friend, 10, 10);
    auto d = MakePeep("elf-mage", Occupacy::Friend, 10, 11);
    auto e = MakePeep("elf-mage", Occupacy::Friend, 10, 10);
    auto mages = MakeGroup("Mages", Occupacy::Friend, { c, d, e });

    auto f = MakePeep("human-peasant", Occupacy::Friend, 15, 9);
    auto g = MakePeep("human-peasant", Occupacy::Friend, 16, 10);
    auto h = MakePeep("human-peasant", Occupacy::Friend, 15, 10);
    auto j = MakePeep("human-peasant", Occupacy::Friend, 15, 11);
    auto k = MakePeep("human-peasant", Occupacy::Friend, 14, 10);
    auto peasants = MakeGroup("Mob", Occupacy::Friend, { f, g, h, j, k });

    auto l = MakePeep("elf-assassin", Occupacy::Friend, 14, 13);
    auto assassin = MakeGroup("Assassin", Occupacy::Friend, { l });

    auto humans = MakeArmy("Humans", Occupacy::Friend, { knights, mages, peasants, assassin });

    auto x = MakePeep("orc-knight", Occupacy::Foe, 30, 11);
    auto y = MakePeep("orc-knight", Occupacy::Foe, 31, 11);
    auto z = MakePeep("orc-knight", Occupacy::Foe, 30, 12);
    auto guards = MakeGroup("Guards", Occupacy::Foe, { x, y, z });

    auto u = MakePeep("orc-warrior", Occupacy::Foe, 30, 15);
    auto s = MakePeep("orc-warrior", Occupacy::Foe, 31, 16);
    auto t = MakePeep("orc-warrior", Occupacy::Foe, 30, 16);
    auto band = MakeGroup("Warband", Occupacy::Foe, { u, s, t });

    auto v = MakePeep("orc-assassin", Occupacy::Foe, 33, 12);
    auto butcher = MakeGroup("Butcher", Occupacy::Foe, { v });

    auto w = MakePeep("orc-assassin", Occupacy::Foe, 33, 15);
    auto executioner = MakeGroup("Executioner", Occupacy::Foe, { w });

    auto p = MakePeep("boar-warrior", Occupacy::Foe, 28, 13);
    auto q = MakePeep("boar-warrior", Occupacy::Foe, 28, 14);
    auto boars = MakeGroup("Boars", Occupacy::Foe, { p, q });

    auto orcs = MakeArmy("Orcs", Occupacy::Foe, { guards, band, butcher, executioner, boars });

    board->armies.push_back(humans);
    board->armies.push_back(orcs);

    for (SInt32 i = 0; i < width; i++)
    {
        for (SInt32 j = 0; j < height; j++)
        {
            if (board->space[i][j] == (UInt32)Occupacy::Empty && Random::Uniform() > chance)
            {
                MakeRandomGround(i, j);
            }
        }
    }
}
