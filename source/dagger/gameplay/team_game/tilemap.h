#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>

using TileProcessor = std::function<Entity(Registry&, UInt32, UInt32,Char)>;
using TilemapLegend = Map<Char, TileProcessor>;
struct Tilemap
{

	Sequence<Entity> tiles;
};
struct TilemapLoadRequest : public AssetLoadRequest<Tilemap>
{
	ViewPtr<TilemapLegend> legend;
	INT32 xOffset, yOffset;
};
String Filename(String path_);