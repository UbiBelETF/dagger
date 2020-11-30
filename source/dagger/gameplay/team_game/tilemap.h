#pragma once

#include "core/system.h"

using namespace dagger;

using TileProcessor = std::function<std::vector<Entity>(Registry&, SInt32, SInt32)>;
using TilemapLegend = Map<Char, TileProcessor>;

struct Tilemap
{
	std::vector<Entity> tiles;
};

struct TilemapLoadRequest : public AssetLoadRequest<Tilemap>
{
	ViewPtr<TilemapLegend> legend;
};

class TilemapSystem : public System, public Subscriber<TilemapLoadRequest>
{
public:
	inline String SystemName() { return "Tilemap System"; }

	void SpinUp() override;
	void WindDown() override;

	void OnLoadAsset(TilemapLoadRequest request_);
};
