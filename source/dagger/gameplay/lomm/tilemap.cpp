#include "tilemap.h"

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

#include <ios>

void TilemapSystem::SpinUp()
{
    Engine::Dispatcher().sink<TilemapLoadRequest>().connect<&TilemapSystem::OnLoadAsset>(this);
}

void TilemapSystem::WindDown()
{
    Engine::Dispatcher().sink<TilemapLoadRequest>().disconnect<&TilemapSystem::OnLoadAsset>(this);

    for (auto tilemap : Engine::Res<Tilemap>())
    {
        for (auto tiles : tilemap.second->tiles)
        {
            for (auto entity : *tiles.second)
            {
                Engine::Registry().destroy(entity);
            }

            delete tiles.second;
        }
    }

    Engine::Res<Tilemap>().clear();
}

void TilemapSystem::OnLoadAsset(TilemapLoadRequest request_)
{
    assert(request_.legend != nullptr);
    Tilemap* tilemap = new Tilemap();

    Char ch;
    UInt32 x = 0, y = 0;

    FileInputStream input{ request_.path };

    while (input >> std::noskipws >> ch) {
        if (ch == '\n')
        {
            x = 0; y++;
            if (tilemap->size.y < y)
                tilemap->size.y = y;
        }
        else
        {
            assert(request_.legend->contains(ch));
            auto result = (request_.legend->at(ch))(Engine::Registry(), x, y);
            UInt64 hash = (UInt64)x * 10000 + (UInt64)y;

            if (tilemap->tiles.find(hash) == tilemap->tiles.end())
                tilemap->tiles[hash] = new Sequence<Entity>{};

            for (auto&& entity : result)
            {
                tilemap->tiles[hash]->emplace_back(std::move(entity));
            }
            x++;
            if (tilemap->size.x < x)
                tilemap->size.x = x;
        }
    }

    Engine::Res<Tilemap>()[request_.path] = tilemap;
}