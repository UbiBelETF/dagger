#include "shoot.h"

#include "core/engine.h"
#include "core/game/transforms.h"
#include "gameplay/common/simple_collisions.h"
#include <math.h>

#include "core/graphics/sprite.h"


using namespace dagger;

using namespace lab;

void lab::CreateBullet(Vector2 position, Float32 ratio, Vector2 directions)
{
	auto& reg = Engine::Instance().Registry();

	auto entity = reg.create();
	auto& bullet = reg.emplace<Bullet>(entity);
	Float32 speedXIntensity = sqrt((bullet.speed * bullet.speed) / (1 + ratio * ratio));
	bullet.speedX = speedXIntensity * directions.x;
	bullet.speedY = speedXIntensity * ratio * directions.y;

	auto& sprite = reg.emplace<Sprite>(entity);
	AssignSprite(sprite, "Blob");
	sprite.size = { 10, 10 };

	auto& transform = reg.emplace<Transform>(entity);
	Float32 distance = sqrt((11 * 11) / (1 + ratio * ratio));
	transform.position.x = position.x + distance * directions.x;
	transform.position.y = position.y + distance * ratio * directions.y;
	transform.position.z = 0.0f;

	auto& collision = reg.emplace<SimpleCollision>(entity);
	collision.size = sprite.size;
}

void lab::ShootingSystem::Run()
{
	auto view = Engine::Registry().view<Transform, Bullet, Sprite,SimpleCollision>();
	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& bullet = view.get<Bullet>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& col = view.get<SimpleCollision>(entity);
		t.position.x += bullet.speedX * Engine::DeltaTime();
		t.position.y += bullet.speedY * Engine::DeltaTime();

		if (col.colided)
		{
			col.colided = false;
			Engine::Registry().remove_all(entity);
		}
	}
}
