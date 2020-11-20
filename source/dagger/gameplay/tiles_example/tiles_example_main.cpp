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
            sprite.position = { i * 16, j * 16, 0 };
        }
    }
}