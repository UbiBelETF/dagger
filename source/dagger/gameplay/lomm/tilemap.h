#pragma once

#include "core/core.h"
#include "core/system.h"
#include <unordered_map>

using TileProcessor = std::function<Sequence<Entity>(Registry&, UInt32, UInt32)>;
using TilemapLegend = Map<Char, TileProcessor>;

struct Tilemap
{
    Vector2 size;
    std::unordered_map<UInt64, Sequence<Entity>*> tiles;
};

struct TilemapLoadRequest : public AssetLoadRequest<Tilemap>
{
	ViewPtr<TilemapLegend> legend;
};

using namespace dagger;

class TilemapSystem
    : public System
    , public Subscriber<TilemapLoadRequest>
{
    String SystemName() override { return "Tilemap System"; }

    void SpinUp() override;
    void WindDown() override;

    void OnLoadAsset(TilemapLoadRequest request_);
};
