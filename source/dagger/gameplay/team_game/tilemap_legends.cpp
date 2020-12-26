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
#include "gameplay/team_game/slime_controller.h"
#include "collision.h"
#include "health_system.h"
#include "attack_system.h"
 int scale = 1;
 int x_step = 16 *scale;
 int y_step = 16 * scale;
 float size = 16 * scale;
using namespace team_game;
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
	creatures['S'] = &CreateSlime;
	creatures['H'] = &CreateHero;
	creatures['B'] = &CreateSlimeBoss;
	objects['c'] = &CreateChest;
	objects['p'] = &CreatePot;
	objects['t'] = &CreateCrate;
	objects['~'] = &CreateDoorHorizontal;
	objects['/'] = &CreateDoorVertical;
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

Entity CreateFloor(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	sprite.scale = { scale,scale };
	sprite.position = { x_ * x_step, y_ * y_step, 1000 };
	sprite.size = Vector2(1, 1) * size;
	AssignSprite(sprite, "spritesheets:tiles_dungeon:floor_1");
	return entity;
}
Entity CreateWall(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Map<Char, String>walls;
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	sprite.subOrigin = Vector2(-0.5, -0.5) * size;
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 10 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:"+wall_type.at(type));
	return entity;
}
Entity CreateSlimeBoss(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);

	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Slime>(entity);
	reg_.emplace<SlimeAi>(entity);
	auto& gameSlime=reg_.emplace<TeamGameSlime>(entity);
	gameSlime.type = "boss";
	auto& health = reg_.emplace<Health>(entity);
	health.maxHp = 150;
	health.hp = 150;
	health.hpBar = reg_.create();
	health.show = true;
	auto& hpBarSprite = reg_.emplace <Sprite>(health.hpBar);
	reg_.emplace<Transform>(health.hpBar);
	hpBarSprite.size = hpBarSprite.size*2.0f;
	hpBarSprite.pivot = Vector2{ 0,-5};
	AssignSprite(hpBarSprite, "TeamGame:enemyHp");
	ATTACH_TO_FSM(SlimeControllerFSM, entity);
	sprite.size = Vector2(1, 1) * size;
	col.size = Vector2(16, 16)*2.0f;
	sprite.scale = { scale*2,scale*2 };
	transform.position = { x_ * x_step, y_ * y_step, 30 };
	AssignSprite(sprite, "spritesheets:chara_slime:slime_idle_anim:1");
	auto& anim = reg_.emplace<Animator>(entity);
	AnimatorPlay(anim, "chara_slime:slime_idle");
	return entity;
}
Entity CreateSlime(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);

	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Slime>(entity);
	reg_.emplace<SlimeAi>(entity);
	reg_.emplace<TeamGameSlime>(entity);
	auto& health = reg_.emplace<Health>(entity);
	health.maxHp = 30;
	health.hp = 30;
	health.hpBar = reg_.create();
	health.show = true;
	auto& hpBarSprite =reg_.emplace <Sprite>(health.hpBar);
	reg_.emplace<Transform>(health.hpBar);
	AssignSprite(hpBarSprite, "TeamGame:enemyHp");
	ATTACH_TO_FSM(SlimeControllerFSM, entity);
	sprite.size = Vector2(1, 1) *( size);
	col.size = Vector2(16,16);
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 30 };
	AssignSprite(sprite, "spritesheets:chara_slime:slime_idle_anim:1");
	auto& anim = reg_.emplace<Animator>(entity);
	AnimatorPlay(anim, "chara_slime:slime_idle");
	return entity;
}
Entity CreateHero(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	auto view = Engine::Registry().view<TeamGameCharacter,Transform,Sprite>();
	auto it = view.begin();
	auto& transform = view.get<Transform>(*it);
	auto& sprite = view.get<Sprite>(*it);
	auto& health = reg_.emplace_or_replace<Health>(*it);
	Entity attackEnt = reg_.create();
	auto& attack= reg_.emplace_or_replace<Attack>(*it);
	auto& attackCol = reg_.emplace_or_replace<Collision>(attackEnt);
	auto& attackTransform = reg_.emplace_or_replace<Transform>(attackEnt);
	auto& attackSprite = reg_.emplace_or_replace<Sprite>(attackEnt);
	AssignSprite(attackSprite, "spritesheets:chara_slime:slime_idle_anim:1");
	attackCol.size = Vector2(5, 5);
	attack.attackEnt = attackEnt;
	attack.offsetVec = Vector3(0, 22, 0);
	attack.damage = 22;
	health.maxHp = 120;
	health.hp = 120;
	health.hpBar = reg_.create();
	health.show = true;
	auto& hpBarSprite = reg_.emplace <Sprite>(health.hpBar);
	reg_.emplace<Transform>(health.hpBar);
	AssignSprite(hpBarSprite, "TeamGame:playerHp");
	sprite.size=Vector2(1, 1) * size;
	sprite.scale = Vector2(scale, scale);
	auto const gameCharacter = view.get<TeamGameCharacter>(*it);
	auto entity = entt::to_entity<Entity,TeamGameCharacter>(reg_, gameCharacter);
	transform.position = { x_ * x_step,y_ * y_step,0 };
	return *it;
}
Entity CreateDoorHorizontal(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 40 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_horizontal");
	return entity;
}
Entity CreateDoorVertical(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 40 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:door_vertical");
	return entity;
}
Entity CreateChest(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:chest");
	return entity;
}
Entity CreatePot(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	auto& create = reg_.emplace<TeamGamePot>(entity);
	auto& health = reg_.emplace<Health>(entity);
	health.maxHp = create.startHp;
	health.hp = create.startHp;
	health.hpBar = reg_.create();
	reg_.emplace <Text>(health.hpBar);
    
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:pot");
	return entity;
}
Entity CreateCrate(Registry& reg_, SInt32 x_, SInt32 y_, char type) {
	Entity entity = reg_.create();
	auto& sprite = reg_.emplace<Sprite>(entity);
	auto& transform = reg_.emplace<Transform>(entity);
	auto& col = reg_.emplace<Collision>(entity);
	reg_.emplace<CollisionType::Wall>(entity);
	auto& pot = reg_.emplace<TeamGameCrate>(entity);
	auto& health = reg_.emplace<Health>(entity);
	health.maxHp = pot.startHp;
	health.hp = pot.startHp;
	health.hpBar = reg_.create();
	reg_.emplace <Text>(health.hpBar);
	
	sprite.size = Vector2(1, 1) * size;
	col.size = sprite.size;
	sprite.scale = { scale,scale };
	transform.position = { x_ * x_step, y_ * y_step, 90 };
	AssignSprite(sprite, "spritesheets:tiles_dungeon:crate");
	return entity;
}