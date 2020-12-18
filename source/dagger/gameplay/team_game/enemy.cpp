#include "enemy.h"
#include "shoot.h"
#include "tilemap_entities.h"

#include "core/graphics/animation.h"
#include "core/input/inputs.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/player_controller_fsm.h"


void lab::EnemySystem::Run()
{
	auto view = Engine::Registry().view<Transform, Skeleton, Sprite, SimpleCollision>();
	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& skeleton = view.get<Skeleton>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& col = view.get<SimpleCollision>(entity);

		auto& reg = Engine::Instance().Registry();

		Vector2 playerPosition;
		auto view3 = reg.view<Sprite, Animator, InputReceiver, Transform, PlayerCharacter, SimpleCollision>();
		for (auto entity : view3)
		{
			auto playerTransform = view3.get<Transform>(entity);
			playerPosition = playerTransform.position;
			playerPosition.y *= -1;
		}

		if (skeleton.type == horizontal)
		{
			if (col.colided && reg.has<Wall>(col.colidedWith))
			{
					col.colided = false;
					skeleton.speed *= -1;
					sprite.scale.x *= -1;
			}
			t.position.x += skeleton.speed * Engine::DeltaTime();
		}

		if (skeleton.type == vertical)
		{
			if (col.colided && reg.has<Wall>(col.colidedWith))
			{
				col.colided = false;
				skeleton.speed *= -1;
			}
			t.position.y -= skeleton.speed * Engine::DeltaTime();
		}

		if (skeleton.type == follower)
		{
			Vector2 directions = { 1, 1 };
			if (playerPosition.x < t.position.x)
				directions.x = -1;
			else if (playerPosition.x == t.position.x)
				directions.x = 0;

			if ((-1 * playerPosition.y) < t.position.y)
				directions.y = -1;
			else if ((-1 * playerPosition.y) == t.position.y)
				directions.y = 0;

			Float32 distanceX = playerPosition.x - t.position.x;
			Float32 distanceY = (playerPosition.y * -1) - t.position.y;
			Float32 ratio = distanceY / distanceX;
			ratio *= (ratio > 0) ? 1 : -1;

			Float32 speedXIntensity = sqrt((skeleton.speed-2) * (skeleton.speed - 2) / (1 + ratio * ratio));
			skeleton.speedX = speedXIntensity * directions.x;
			skeleton.speedY = speedXIntensity * ratio * directions.y;

			t.position.x += skeleton.speedX * Engine::DeltaTime();
			t.position.y += skeleton.speedY * Engine::DeltaTime();
		}

		if (skeleton.health <= 0)
		{
			reg.remove_all(entity);
		}

		if (col.colided)
		{
			auto view2 = reg.view<Bullet, Sprite, Transform, SimpleCollision>();
			for (auto entity : view2)
			{
				if (entity == col.colidedWith)
				{
					Bullet bullet = view2.get<Bullet>(entity);
					if(bullet.ownership == player)
						skeleton.health -= bullet.damage;
					col.colided = false;
				}
			}
		}

		if (skeleton.cooldown <= 0)
		{
			CreateBullet(t.position, playerPosition, Unit::skeleton);
			skeleton.cooldown = skeleton.maxCooldown;
		}
		else
			skeleton.cooldown--;
	}
}