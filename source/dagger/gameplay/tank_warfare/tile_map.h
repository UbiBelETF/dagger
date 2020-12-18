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

	Entity CreateGrass(Registry&, SInt32, SInt32);
	Entity CreateDirt(Registry&, SInt32, SInt32);
	Entity CreateBiggestBuilding(Registry&, SInt32, SInt32);
	Entity CreateBankBuilding(Registry&, SInt32, SInt32);
	Entity CreateSmallestBuilding(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateShopBuilding(Registry& reg, SInt32 x_, SInt32 y_);
	Entity CreateBuilding(Registry& reg, SInt32 x_, SInt32 y_);
	Entity CreateGroupTrees(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateTree(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateMediumBuilding(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateHorizontalWall(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateVerticallWall(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateStorage(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateSideFence(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateFrontFence(Registry& reg_, SInt32 x_, SInt32 y_);
	Entity CreateCarBack(Registry& reg_, SInt32 x_, SInt32 y_);


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