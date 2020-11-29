#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/character_controller.h"

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

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<TeamGameControllerSystem>();
}

struct MainCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    TeamGameCharacter& character;
    Transform& transform;

    static MainCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<TeamGameCharacter>(entity);
        auto& transform = reg.emplace<Transform>(entity);
        reg.emplace<ControllerMapping>(entity);
        return MainCharacter{ entity, sprite, anim, input, character, transform };
    }

    static MainCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = MainCharacter::Get(entity);

        chr.sprite.scale = { 3, 3 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "spritesheets:chara_hero:hero_idle_anim:1");
        AnimatorPlay(chr.animator, "chara_hero:hero_idle");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 100;

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
    auto& reg = Engine::Registry();
    for (int i = -50; i < 50; i++)
    {
        for (int j = -50; j < 50; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, fmt::format("spritesheets:tiles_dungeon:floor_{}", 1 + (rand() % 12)));
            sprite.position = { i * 16, j * 16, 99 };
        }
    }

    String anims[] = { "idle", "move_up", "move_down", "move_side", "attack_up", "attack_side", "attack_down", "hit_up", "hit_down", "hit_side" };
    for (int i = 0; i < 10 ; i++)
    {
        auto slime = reg.create();
        auto& sprite = reg.emplace<Sprite>(slime);
        AssignSprite(sprite, "spritesheets:chara_slime:slime_"+anims[i%10]+"_anim:1");
        sprite.position = { rand() % 300 -300, rand() % 300 - 150, 0 };
        sprite.position.z = (150.0f + sprite.position.y) / 10.0f;
        sprite.scale = { 3, 3 };

        auto& anim = reg.emplace<Animator>(slime);
        AnimatorPlay(anim, "chara_slime:slime_"+anims[i%10]);
    }
    for (int i = 0; i < 10; i++)
    {
        auto hero = reg.create();
        auto& sprite = reg.emplace<Sprite>(hero);
        AssignSprite(sprite, "spritesheets:chara_hero:hero_" + anims[i % 10] + "_anim:1");
        sprite.position = { rand() % 300 , rand() % 300 -150 , 0 };
        sprite.position.z = (150.0f + sprite.position.y) / 10.0f;
        sprite.scale = { 3, 3 };

        auto& anim = reg.emplace<Animator>(hero);
        AnimatorPlay(anim, "chara_hero:hero_" + anims[i % 10]);
    }

    auto ui = reg.create();
    auto& text = reg.emplace<Text>(ui);
    text.spacing = 0.6f;
    text.Set("pixel-font", "hello world");
}
