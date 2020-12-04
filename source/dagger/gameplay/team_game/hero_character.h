#pragma once
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
#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/hero_controller.h"

using namespace team_game;

struct MainCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    TeamGameCharacter& character;

    static MainCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<TeamGameCharacter>(entity);
        return MainCharacter{ entity, sprite, anim, input, character };
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

        return chr;
    }
};
