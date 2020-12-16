#include "enemy.h"
#include "shoot.h"

#include "core/graphics/animation.h"
#include "core/input/inputs.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/player_controller_fsm.h"


void lab::EnemySystem::Run()
{
	auto view = Engine::Registry().view<Transform, Bandit, Sprite, SimpleCollision>();
	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& bandit = view.get<Bandit>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& col = view.get<SimpleCollision>(entity);

		auto& reg = Engine::Instance().Registry();

		if (bandit.health <= 0)
		{
			reg.remove_all(entity);
		}

		if (col.colided)
		{
			col.colided = false;
			auto view2 = reg.view<Bullet, Sprite, Transform, SimpleCollision>();
			int damage;
			for (auto entity : view2)
			{
				if (entity == col.colidedWith)
				{
					Bullet bullet = view2.get<Bullet>(entity);
					bandit.health -= bullet.damage;
				}
			}
		}

		if (bandit.cooldown <= 0)
		{
			auto view3 = reg.view<Sprite, Animator, InputReceiver, Transform, PlayerCharacter, SimpleCollision>();
			Vector2 playerPosition;
			for (auto entity : view3)
			{
				auto playerTransform = view3.get<Transform>(entity);
				playerPosition = playerTransform.position;
				playerPosition.y *= -1;
				CreateBullet(t.position, playerPosition);
			}
			

			bandit.cooldown = bandit.maxCooldown;
		}
		else
			bandit.cooldown--;

	}
}