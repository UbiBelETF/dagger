#include "lomm_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/lomm/tilemap.h"

using namespace dagger;

void lomm::LommGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<TilemapSystem>();
}

Sequence<Entity> CreateGrass(Registry& registry_, UInt32 x_, UInt32 y_)
{
    auto entity = registry_.create();
    registry_.emplace<lomm::Ground>(entity);
    return { entity };
}

Sequence<Entity> CreateWater(Registry& registry_, UInt32 x_, UInt32 y_)
{
    auto entity = registry_.create();
    registry_.emplace<lomm::Water>(entity);
    return { entity };
}

void lomm::LommGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 1200, 800 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    TilemapLegend legend;
    legend['.'] = &CreateGrass;
    legend['~'] = &CreateWater;

    Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/island.tilemap", &legend });
    auto tilemap = Engine::Res<Tilemap>()["tilemaps/island.tilemap"];

    Registry& reg = Engine::Registry();

    for (int i = 0; i < tilemap->size.x; i++)
    {
        for (int j = 0; j < tilemap->size.y; j++)
        {
            UInt64 hash = (UInt64)i * 10000 + (UInt64)j;

            if (tilemap->tiles.find(hash) != tilemap->tiles.end())
            {
                Entity tile = tilemap->tiles[hash]->at(0);
                if (reg.has<lomm::Ground>(tile))
                {

                }
            }
        }

    }
}
