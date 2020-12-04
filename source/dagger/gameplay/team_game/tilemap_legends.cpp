#include "tilemap_legends.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/text.h"
#include "core/graphics/animations.h"
Tilemap_legends::Tilemap_legends() {
	TilemapLegend floors;
	TilemapLegend creatures;
	TilemapLegend objects;
	floors['.'] = &CreateFloor;
	floors['='] = &CreateWall;
	floors['|'] = &CreateWall;
	floors['1'] = &CreateWall;
	floors['2'] = &CreateWall;
	floors['3'] = &CreateWall;
	floors['4'] = &CreateWall;
	floors['~'] = &CreateDoorHorizontal;
	floors['\\'] = &CreateDoorVertical;
	creatures['S'] = &CreateSlime;
	creatures['H'] = &CreateHero;
	objects['c'] = &CreateChest;
	objects['p'] = &CreatePot;
	objects['t'] = &CreateCrate;
	wall_type['='] = "wall_1";
	wall_type['|'] = "wall_11";
	wall_type['1'] = "wall_7";
	wall_type['2'] = "wall_8";
	wall_type['3'] = "wall_12";
	wall_type['4'] = "wall_13";
}
Entity CreateFloor(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:floor_1");
}
Entity CreateWall(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Map<Char, String>walls;
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:"+wall_type.at(type));

}
Entity CreateSlime(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:chara_slime:slime_idle_anim:1");
	auto& anim = reg_.emplace<Animator>(entity);
	AnimatorPlay(anim, "chara_slime:slime_idle");

}
Entity CreateHero(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:chara_hero:hero_idle_anim:1");
	auto& anim = reg_.emplace<Animator>(entity);
	AnimatorPlay(anim, "chara_hero:hero_idle");
}
Entity CreateDoorHorizontal(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_horizontal");
}
Entity CreateDoorVertical(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_vertical");
}
Entity CreateChest(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:chest");
}
Entity CreatePot(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:pot");
}
Entity CreateCrate(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.position = { x_ * 16, y_ * 16, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:crate");
}