#include "chests.h"

#include <math.h>
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"


using namespace dagger;
using namespace lab;

void lab::CreateChest(Vector2 position, Float32 hpRestored = 50)
{
	auto& reg = Engine::Instance().Registry();
	auto entity = reg.create();

	auto& chest = reg.emplace<Chest>(entity);
	chest.hpRestored = hpRestored;
	chest.empty = false;

	auto& sprite = reg.emplace<Sprite>(entity);
	AssignSprite(sprite, "chest:chest_closed");
	sprite.size = { 30, 30 };

	auto& transform = reg.emplace<Transform>(entity);
	transform.position.x = position.x;
	transform.position.y = position.y;

	auto& col = reg.emplace<SimpleCollision>(entity);
	col.size = sprite.size;
}
