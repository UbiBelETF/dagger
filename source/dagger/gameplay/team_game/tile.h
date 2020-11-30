#pragma once

#include "core/core.h"
#include "core/system.h"
#include <functional>


using TileProcessor = std::function<Entity(Registry&, UInt32, UInt32)>; 
using TilemapLegend = Map<Char, TileProcessor>;

struct Tilemap
{
  Sequence<Entity> tiles;
};

struct TilemapLoadRequest : public AssetLoadRequest<Tilemap>
{
    ViewPtr<TilemapLegend> legend;
};