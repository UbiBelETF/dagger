#pragma once
#include<functional>
#include<map>
#include"core/core.h"
#include"core/system.h"

using namespace dagger;
namespace tank_warfare
{
	using TileProcessor = std::function<Entity(Registry&, SInt32, SInt32)>;
	using TilemapLegend = std::map<Char, TileProcessor>;

	Entity EmptyCollision(Registry&, SInt32, SInt32);
	Entity CreateBiggestBuilding(Registry&, SInt32, SInt32);
	Entity CreateBankBuilding(Registry&, SInt32, SInt32);
	Entity CreateSmallestBuilding(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateBuilding(Registry& reg, SInt32 x_, SInt32 y_);
	Entity CreateGroupTrees(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateMediumBuilding(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateStorage(Registry& reg_, SInt32 x_, SInt32 y_);

	void MapLoading();

	struct Tilemap
	{
		Sequence<Entity> tiles;
		static TilemapLegend legend;
	};

	struct TilemapLoadRequest : public AssetLoadRequest<Tilemap>
	{
		ViewPtr<TilemapLegend> tile_legend;
	};

	class TilemapSystem
		: public System
		, public Subscriber<TilemapLoadRequest>
	{
	public:
		String SystemName() override
		{
			return "Tilemap System";
		}
		void SpinUp() override;
		void WindDown() override;

		void OnLoadAsset(TilemapLoadRequest request_);
	};
}