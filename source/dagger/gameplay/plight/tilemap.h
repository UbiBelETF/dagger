#include <functional>

#include "core/engine.h"

using TileProcessor = std::function<Entity(Registry&, INT32, INT32)>;
using TilemapLegend = Map<Char, TileProcessor>;

struct Tilemap {
	Sequence<Entity> tiles;
};

struct TilemapLoadRequest : public AssetLoadRequest<Tilemap> {
	ViewPtr<TilemapLegend> legend;
};