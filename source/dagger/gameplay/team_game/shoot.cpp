#include "gameplay/team_game/shoot.h"
#include "gameplay/team_game/collectables.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/player_controller_fsm.h"
#include "gameplay/common/simple_collisions.h"
#include <math.h>

#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"




using namespace dagger;

using namespace lab;

void lab::CreateBullet(Vector2 position, Vector2 target, Unit owner, String spritename)
{
	auto& reg = Engine::Instance().Registry();

	auto entity = reg.create();
	auto& bullet = reg.emplace<Bullet>(entity);
	bullet.ownership = owner;

	switch (owner)
	{
	case player: bullet.damage = 15; break;
	case skeleton: bullet.damage = 5; break;
	case slime: bullet.damage = 15; break;
	}

	Vector2 directions = { 1, 1 };
	if (target.x < position.x)
		directions.x = -1;
	else if (target.x == position.x)
		directions.x = 0;

	if ((-1 * target.y) < position.y)
		directions.y = -1;
	else if ((-1 * target.y) == position.y)
		directions.y = 0;

	Float32 distanceX = target.x - position.x;
	Float32 distanceY = (target.y * -1) - position.y;
	Float32 ratio = distanceY / distanceX;
	ratio *= (ratio > 0) ? 1 : -1;

	Float32 speedXIntensity = sqrt((bullet.speed * bullet.speed) / (1 + ratio * ratio));
	bullet.speedX = speedXIntensity * directions.x;
	bullet.speedY = speedXIntensity * ratio * directions.y;

	auto& sprite = reg.emplace<Sprite>(entity);
	AssignSprite(sprite, spritename);
	sprite.size = { 30.f, 30.f };

	auto& transform = reg.emplace<Transform>(entity);
	Float32 distance = sqrt((15 * 15) / (1 + ratio * ratio));
	transform.position.x = position.x + distance * directions.x;
	transform.position.y = position.y + distance * ratio * directions.y;
	transform.position.z = 0.0f;

	auto& collision = reg.emplace<SimpleCollision>(entity);
	collision.size.x = 1;
	collision.size.y = 1;
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
			auto& reg = Engine::Instance().Registry();
			if (!reg.has<Bullet>(col.colidedWith) && !reg.has<Heart>(col.colidedWith))
			{
				Engine::Registry().remove_all(entity);
			}
		}
	}

	auto gunView = Engine::Registry().view<Transform, Sprite, Gun>();
	for (auto entity1 : gunView)
	{
		Vector2 playerPosition;
		Float32 gunScaleX = 1;
		auto playerView = Engine::Registry().view<Transform, Sprite, Animator, InputReceiver, PlayerCharacter, NextLvl, SimpleCollision>();
		for (auto entity2 : playerView)
		{
			auto& t = Engine::Registry().get<Transform>(entity2);
			playerPosition.x = t.position.x;
			playerPosition.y = t.position.y;

			auto& s = Engine::Registry().get<Sprite>(entity2);
			gunScaleX = s.scale.x;
		}
		auto& gunT = Engine::Registry().get<Transform>(entity1);
		gunT.position = { playerPosition , 0.0f };

		auto& gunS = Engine::Registry().get<Sprite>(entity1);
		gunS.scale.x = gunScaleX;
	}
}
