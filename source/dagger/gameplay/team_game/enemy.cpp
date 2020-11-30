#include "enemy.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"

void lab::EnemySystem::Run()
{
	auto view = Engine::Registry().view<Transform, Bullet, Sprite>();
}
