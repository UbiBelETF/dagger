#include<ios>
#include<iostream>
#include<regex>
#include "tile_map.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"

using namespace dagger;

tank_warfare::TilemapLegend
tank_warfare::Tilemap::legend;

Entity tank_warfare::CreateBuilding(Registry& reg_, SInt32 x_, SInt32 y_) {
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", 2));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.5;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::CreateBankBuilding(Registry& reg_, SInt32 x_, SInt32 y_) {
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", 8));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.5;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::CreateBiggestBuilding(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", (rand() % 4) + 5));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.5;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::CreateSmallestBuilding(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", (rand() % 15) + 15));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.5;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::CreateMediumBuilding(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", (rand() % 6) + 9));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.7;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::CreateGroupTrees(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:tree:tree{}", (rand() % 2) + 1));
	sprite.position = { x_ * 48, y_ * 48, 2 };
	sprite.size *= 1.3;
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 2 };
	return entity;
}

Entity tank_warfare::CreateStorage(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& col = reg_.emplace<SimpleCollision>(entity);
	AssignSprite(sprite, fmt::format("jovanovici:building:building{}", (rand() % 2) + 5));
	sprite.position = { x_ * 48, y_ * 48, 1.5 };
	sprite.size *= 1.3;
	col.size.x = sprite.size.x;
	col.size.y = sprite.size.y / 2;
	col.pivot = { -0.5f,-0.8f };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

Entity tank_warfare::EmptyCollision(Registry& reg_, SInt32 x_, SInt32 y_)
{
	auto entity = reg_.create();
	auto& col = reg_.emplace<SimpleCollision>(entity);
	col.size = { 18, 64 };
	auto& tra = reg_.emplace<Transform>(entity);
	tra.position = { x_ * 48, y_ * 48, 1.5 };
	return entity;
}

void tank_warfare::TilemapSystem::SpinUp()
{
	Engine::Dispatcher().sink<TilemapLoadRequest>().connect<&TilemapSystem::OnLoadAsset>(this);
}

void tank_warfare::TilemapSystem::OnLoadAsset(TilemapLoadRequest request_) {
	assert(request_.tile_legend != nullptr);
	Tilemap *tilemap = new Tilemap();

	FileInputStream input{ request_.path };

	Char ch;

	SInt32 xSize;
	input >> std::skipws >> xSize;
	xSize = (-1) * xSize / 2;

	SInt32 ySize;
	input >> std::skipws >> ySize;
	ySize = (-1) * ySize / 2;

	SInt32 x = xSize, y = ySize;

	while (input >> std::noskipws >> ch)
	{
		if (ch == '\n')
		{
			x = xSize; y++;
		}
		else
		{
			if (ch != '~') {
				assert(request_.tile_legend->at(ch));
				tilemap->tiles.push_back((request_.tile_legend->at(ch))(Engine::Registry(), x, y));
			}
			x++;
		}
	}

	Engine::Res<Tilemap>()[request_.path] = tilemap;
}

void tank_warfare::TilemapSystem::WindDown()
{
	auto& tilemaps = Engine::Res<Tilemap>();
	for (auto t = tilemaps.begin(); t != tilemaps.end(); t++)
	{
		delete t->second;
	}

	tilemaps.clear();

	Engine::Dispatcher().sink<TilemapLoadRequest>().disconnect<&TilemapSystem::OnLoadAsset>(this);
}
