#include "tiles_example_main.h"

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
using namespace tiles_example;

void TilesExampleMain::GameplaySystemsSetup(Engine& engine_) 
{}

void TilesExampleMain::WorldSetup(Engine& engine_) 
{
    auto& reg = Engine::Registry();
    for (int i = -50; i < 50; i++)
    {
        for (int j = -50; j < 50; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, fmt::format("spritesheets:dungeon:floor_{}", 1 + (rand() % 8)));
            sprite.position = { i * 16, j * 16, 99 };
        }
    }

    for (int i = 0; i < 10; i++)
    {
        auto goblin = reg.create();
        auto& sprite = reg.emplace<Sprite>(goblin);
        AssignSprite(sprite, "spritesheets:dungeon:goblin_idle_anim:1");
        sprite.position = { rand() % 300 - 150, rand() % 300 - 150, 0 };
        sprite.position.z = (150.0f + sprite.position.y) / 10.0f;
        sprite.scale = { 3, 3 };

        auto& anim = reg.emplace<Animator>(goblin);
        AnimatorPlay(anim, "dungeon:goblin_idle");
    }

    auto ui = reg.create();
    auto& text = reg.emplace<Text>(ui);
    text.spacing = 0.6f;
    text.Set("pixel-font", "hello world");
    text.Set("pixel-font", "new text",{10,-200,0});

}