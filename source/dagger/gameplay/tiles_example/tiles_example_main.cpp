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

void ClickSpawnerSystem::Run()
{
    if (dagger::Input::IsInputDown(EDaggerMouse::MouseButton1))
    {
        auto& reg = Engine::Registry();
        auto goblin = reg.create();
        auto& sprite = reg.emplace<Sprite>(goblin);
        AssignSprite(sprite, "spritesheets:dungeon:goblin_idle_anim:1");
        Vector2 pos = dagger::Input::CursorPositionInWorld();
        sprite.position = { pos.x, pos.y, 0.0f };
        sprite.position.z = 5000.0f + sprite.position.y;
        sprite.scale = { 3, 3 };

        auto& anim = reg.emplace<Animator>(goblin);
        AnimatorPlay(anim, "dungeon:goblin_idle");

        dagger::Input::ConsumeInput(EDaggerMouse::MouseButton1);
    }
}

void TilesExampleMain::GameplaySystemsSetup(Engine& engine_) 
{
    engine_.AddSystem<ClickSpawnerSystem>();
}

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
            sprite.position = { i * 16, j * 16, 9999 };
        }
    }

    auto ui = reg.create();
    auto& text = reg.emplace<Text>(ui);
    text.spacing = 0.6f;
    text.Set("pixel-font", "hello world");
}