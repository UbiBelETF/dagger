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
};
String Filename(String path_) {
	String base_name = path_.substr(path_.find_last_of("\//") + 1);
	size_t const dot = base_name.find_first_of('.');
	String filename = base_name.substr(0, dot);
	return filename;
}