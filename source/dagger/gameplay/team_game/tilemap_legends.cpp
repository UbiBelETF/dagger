#include "tilemap_legends.h"
#include "core/input/inputs.h"
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
#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/hero_controller.h"

const int scale = 3;
const int x_step = 16 *scale;
const int y_step = 16 * scale;
using namespace team_game;


// You need to learn a lot about abstraction. You have so much repetition in this one file that it's basically
// sure to get rejected in any serious project. So let's learn.

// Here we make a function that returns a function. You basically look at all the CreateFloor, CreateDoor etc.
// functions below and think "what is DIFFERENT about all of these SAME functions?" and pull that out.
// Here, we can see that it's sprite kind and depth, mostly, so we do that.
std::function<Entity(Registry&, UInt32, UInt32, char)> MakeSpriteEntity(String spriteKind_, UInt32 depth_)
{
	return [spriteKind_, depth_](Registry& reg_, UInt32 x_, UInt32 y_, char type_)
	{
		Entity entity = reg_.create();
		auto& sprite = reg_.emplace<Sprite>(entity);
		sprite.scale = { scale,scale };
		sprite.position = { x_ * x_step, y_ * y_step, depth_ };
		AssignSprite(sprite, spriteKind_);
		return entity;
	};
}

Tilemap_legends::Tilemap_legends() {
	TilemapLegend floors;
	TilemapLegend creatures;
	TilemapLegend objects;

	// Now we use that instead of writing 50 functions...
	floors['.'] = MakeSpriteEntity("spritesheets:tiles_dungeon:floor_1", 1000);
	objects['~'] = MakeSpriteEntity("spritesheets:tiles_dungeon:door_horizontal", 40);
	objects['/'] = MakeSpriteEntity("spritesheets:tiles_dungeon:door_vertical", 40);

	floors['='] = &CreateWall;
	floors['|'] = &CreateWall;
	floors['1'] = &CreateWall;
	floors['2'] = &CreateWall;
	floors['3'] = &CreateWall;
	floors['4'] = &CreateWall;
	creatures['S'] = &CreateSlime;
	creatures['H'] = &CreateHero;
	objects['c'] = &CreateChest;
	objects['p'] = &CreatePot;
	objects['t'] = &CreateCrate;
	wall_type['='] = "wall_2";
	wall_type['|'] = "wall_11";
	wall_type['1'] = "wall_7";
	wall_type['2'] = "wall_8";
	wall_type['3'] = "wall_12";
	wall_type['4'] = "wall_13";
	legends["floors"] = floors;
	legends["creatures"] = creatures;
	legends["objects"] = objects;
}

// We don't need 70% of these, and even those we do need can be made shorter 
// because you return the entity, so you can just do some extra steps.

Entity CreateWall(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Map<Char, String>walls;
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 10 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:"+wall_type.at(type));
	return entity;
}

Entity CreateSlime(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 30 };
	AssignSprite(sprite, "spritesheets:chara_slime:slime_idle_anim:1");
	auto& anim = reg_.emplace<Animator>(entity);
	AnimatorPlay(anim, "chara_slime:slime_idle");
	return entity;
}
Entity CreateHero(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	auto view = Engine::Registry().view<TeamGameCharacter,Sprite>();
	auto it = view.begin();
	auto& sprite = view.get<Sprite>(*it);
	sprite.position = { x_ * x_step,y_ * y_step,sprite.position.z };
	return *it;
}
Entity CreateDoorHorizontal(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 40 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_horizontal");
	return entity;
}
Entity CreateDoorVertical(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 40 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_vertical");
	return entity;
}
Entity CreateChest(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:chest");
	return entity;
}
Entity CreatePot(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:pot");
	return entity;
}
Entity CreateCrate(Registry& reg_, UInt32 x_, UInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:crate");
	return entity;
}