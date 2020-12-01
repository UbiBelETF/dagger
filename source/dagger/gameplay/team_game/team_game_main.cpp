#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/hero_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"
#include "core/graphics/text.h"

#include "gameplay/team_game/character_collision.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<CharacterCollisionSystem>();
    engine_.AddSystem<TeamGameControllerSystem>();
}

struct MainCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    TeamGameCharacter& character;
    CharacterCollision& col;
    Transform& tm;


    static MainCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<TeamGameCharacter>(entity);
        auto& col = reg.get_or_emplace<CharacterCollision>(entity);
        auto& tm = reg.get_or_emplace<Transform>(entity);
        return MainCharacter{ entity, sprite, anim, input, character, col, tm };
    }

    static MainCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = MainCharacter::Get(entity);

        ATTACH_TO_FSM(HeroControllerFSM, entity);

        chr.sprite.scale = { 3, 3 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "spritesheets:chara_hero:hero_idle_anim:1");
        AnimatorPlay(chr.animator, "chara_hero:hero_idle");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 100;

        //Collision setup:        
        chr.col.size = { 25, 28 };
        chr.tm.position = { position_ , 0.0f };
        CharacterCollisionSystem::MainCharacterEntity = entity;

        return chr;
    }
};

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto mainChar = MainCharacter::Create("ASDW", { 1, 1, 1 }, { 0, 0 });


    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

void team_game::SetupWorld(Engine &engine_)
{
    //TODO: Setup world

    //generate collision test world:
    auto& reg = Engine::Registry();
    for (int i = -4; i < 4; i++)
    {
        for (int j = -4; j < 4; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:floor_{}", 1));
            sprite.position = { i * 48, j * 48, 99 };
            sprite.scale = { 3, 3 };

        }
    }
    for (int i = -5; i < 5; i++) {
        if (i == 0) continue;
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:wall_{}", i == -5 ? 12 : i == 4 ? 13 : i == 1 ? 2 : i == -1 ? 4 : 3));
        sprite.position = { i * 48, -5 * 48, 99 };
        sprite.scale = { 3, 3 };

        auto& col = reg.emplace<CharacterCollision>(entity);
        auto& tm = reg.emplace<Transform>(entity);
        col.size = { 48, 48 };
        tm.position = sprite.position;
    }
    for (int i = -5; i < 5; i++) {
        if (i == 0) continue;
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:wall_{}", i == -5 ? 7 : i == 4 ? 8 : i == 1 ? 2 : i == -1 ? 4 : 3));
        sprite.position = { i * 48, 4 * 48, 99 };
        sprite.scale = { 3, 3 };

        auto& col = reg.emplace<CharacterCollision>(entity);
        auto& tm = reg.emplace<Transform>(entity);
        col.size = { 48, 48 };
        tm.position = sprite.position;
    }
    for (int i = -4; i < 4; i++) {
        if (i == 0) continue;
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:wall_{}", i == 1 ? 16 : i == -1 ? 6 : 11));
        sprite.position = { -5 * 48, i * 48, 99 };
        sprite.scale = { 3, 3 };

        auto& col = reg.emplace<CharacterCollision>(entity);
        auto& tm = reg.emplace<Transform>(entity);
        col.size = { 48, 48 };
        tm.position = sprite.position;
    }

    for (int i = -4; i < 4; i++) {
        if (i == 0) continue;
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:wall_{}", i == 1 ? 16 : i == -1 ? 6 : 11));
        sprite.position = { 4 * 48, i * 48, 99 };
        sprite.scale = { 3, 3 };

        auto& col = reg.emplace<CharacterCollision>(entity);
        auto& tm = reg.emplace<Transform>(entity);
        col.size = { 48, 48 };
        tm.position = sprite.position;
    }
}
