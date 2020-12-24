#include "gameplay/team_game/enemy.h"
#include "gameplay/team_game/shoot.h"
#include "gameplay/team_game/tilemap_entities.h"
#include "gameplay/team_game/collectables.h"

#include "core/graphics/animation.h"
#include "core/input/inputs.h"
#include "core/engine.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/text.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/player_controller_fsm.h"

int lab::EnemySystem::bossesAlive = 3;

void lab::EnemySystem::Run()
{
	Vector2 playerPosition;
	auto view3 = Engine::Registry().view<Sprite, Animator, InputReceiver, Transform, PlayerCharacter, SimpleCollision, Text>();
	for (auto entity : view3)
	{
		auto playerTransform = view3.get<Transform>(entity);
		playerPosition = playerTransform.position;
		playerPosition.y *= -1;

		if (bossesAlive <= 0)
		{
			auto textEntity = Engine::Registry().create();
			auto& txt = Engine::Registry().emplace<Text>(textEntity);
			txt.position = { 50, 0, 0 };
			txt.spacing = 0.8f;
			txt.alignment = { TextAlignment::CENTER };
			txt.Set("pixel-font", "VICTORY!", { 30,30 });
		}
	}

	auto viewCollisions = Engine::Registry().view<Transform, SimpleCollision>();

  auto view = Engine::Registry().view<Transform, Skeleton, Sprite, SimpleCollision, Animator>();
	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& skeleton = view.get<Skeleton>(entity);
		auto& sprite = view.get<Sprite>(entity);
		auto& col = view.get<SimpleCollision>(entity);
		auto& animator = view.get<Animator>(entity);

		auto& reg = Engine::Instance().Registry();

		if ((skeleton.type != boss1 && skeleton.type != boss2 && skeleton.health <= 0) || ((skeleton.type == boss1 || skeleton.type == boss2) && skeleton.bossHealth <= 0))
		{
			AnimatorPlay(animator, "skeleton:death");
			skeleton.deathTimer--;
			if (skeleton.deathTimer <= 0)
			{
				if (skeleton.type == boss1 || skeleton.type == boss2)
					bossesAlive--;
				reg.remove_all(entity);
			}
		}
		else
		{

			if (skeleton.type == horizontal)
			{
				if (col.colided && reg.has<Wall>(col.colidedWith))
				{
					col.colided = false;
					skeleton.speed *= -1;
					sprite.scale.x *= -1;
				}
				if (skeleton.health > 0)
					t.position.x += skeleton.speed * Engine::DeltaTime();
			}

			else if (skeleton.type == vertical)
			{
				if (col.colided && reg.has<Wall>(col.colidedWith))
				{
					col.colided = false;
					skeleton.speed *= -1;
				}
				if (skeleton.health > 0)
					t.position.y -= skeleton.speed * Engine::DeltaTime();
			}

			else if (skeleton.type == follower)
			{
				if (col.colided && !Engine::Registry().has<Bullet>(col.colidedWith) && !Engine::Registry().has<Heart>(col.colidedWith))
				{
					if(Engine::Registry().valid(col.colidedWith) && Engine::Registry().has<Transform>(col.colidedWith))
					{
					
						SimpleCollision& collision = Engine::Registry().get<SimpleCollision>(col.colidedWith);
						
						Transform& transform = Engine::Registry().get<Transform>(col.colidedWith);

						Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

						do
						{
							Float32 dt = Engine::DeltaTime();
							if (collisionSides.x > 0)
							{
								t.position.x -= ((skeleton.speed - 2) * dt);
							}

							if (collisionSides.y > 0)
							{
								t.position.y -= ((skeleton.speed - 2) * dt);
							}
							if (collisionSides.x < 0)
							{
								t.position.x += ((skeleton.speed - 2) * dt);
							}

							if (collisionSides.y < 0)
							{
								t.position.y += ((skeleton.speed - 2) * dt);
							}
						} while (col.IsCollided(t.position, collision, transform.position));
					
					}
					col.colided = false;
				}
				else
				{
					Vector2 speed = speedForFollow(t.position, playerPosition, skeleton.speed);
					skeleton.speedX = speed.x;
					skeleton.speedY = speed.y;

					if (skeleton.health > 0)
					{
						t.position.x += skeleton.speedX * Engine::DeltaTime();
						t.position.y += skeleton.speedY * Engine::DeltaTime();
						if (t.position.x > playerPosition.x)
							sprite.scale.x = -1;
						else
							sprite.scale.x = 1;
					}
				}
			}

			else if (skeleton.type == boss1)
			{
				if (col.colided && reg.has<Wall>(col.colidedWith))
				{
					col.colided = false;
					skeleton.speed *= -1;
					sprite.scale.x *= -1;
				}
				if (skeleton.health > 0)
					t.position.x += 4 * skeleton.speed * Engine::DeltaTime();
			}

			else if (skeleton.type == boss2)
			{
				if (col.colided && reg.has<Wall>(col.colidedWith))
				{
					col.colided = false;
					skeleton.speed *= -1;
				}
				if (skeleton.health > 0)
					t.position.y -= 2 * skeleton.speed * Engine::DeltaTime();
				if (t.position.x > playerPosition.x)
					sprite.scale.x = -1;
				else
					sprite.scale.x = 1;
			}

			if (col.colided)
			{
				auto view2 = reg.view<Bullet, Sprite, Transform, SimpleCollision>();
				for (auto entity : view2)
				{
					if (entity == col.colidedWith)
					{
						Bullet bullet = view2.get<Bullet>(entity);
						if (bullet.ownership != Unit::skeleton && skeleton.type != boss1 && skeleton.type != boss2)
							skeleton.health -= bullet.damage;
						if (bullet.ownership == Unit::player && (skeleton.type == boss1 || skeleton.type == boss2))
							skeleton.bossHealth -= bullet.damage;
						col.colided = false;
					}
				}
			}

			if (skeleton.cooldown <= 0)
			{
				if (skeleton.type == horizontal || skeleton.type == vertical)
					CreateBullet(t.position, playerPosition, Unit::skeleton, "Bullet");
				else if (skeleton.type == boss1)
				{
					for (int i = 0; i < 10; i++)
					{
						switch (i)
						{
						case 0: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
						case 1: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
						case 2: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
						case 3: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
						case 4: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
						case 5: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
						case 6: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
						case 7: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
						case 8: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
						case 9: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
						}
					}
				}
				else if (skeleton.type == boss2)
				{
					for (int i = 0; i < 10; i++)
					{
						if (t.position.x < 0)
						{
							switch (i)
							{
							case 0: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 1: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 2: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 3: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 4: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 5: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 6: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 7: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 8: CreateBullet(t.position, { rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 9: CreateBullet(t.position, { rand(), rand() }, Unit::Boss, "Bullet"); break;
							}
						}
						else
						{
							switch (i)
							{
							case 0: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 1: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 2: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 3: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 4: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 5: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 6: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
							case 7: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 8: CreateBullet(t.position, { -rand(), -rand() }, Unit::Boss, "Bullet"); break;
							case 9: CreateBullet(t.position, { -rand(), rand() }, Unit::Boss, "Bullet"); break;
							}
						}
					}
				}
				skeleton.cooldown = skeleton.maxCooldown;
			}
			else
				skeleton.cooldown--;

			
			
		}
	}

	auto slimeView = Engine::Registry().view<Transform, Slime, Sprite, SimpleCollision, Animator>();
	for (auto entity : slimeView)
	{
		auto& slime = Engine::Registry().get<Slime>(entity);
		auto& t = Engine::Registry().get<Transform>(entity);
		auto& s = Engine::Registry().get<Sprite>(entity);
		auto& col = Engine::Registry().get<SimpleCollision>(entity);
		auto& animator = Engine::Registry().get <Animator>(entity);

		if (slime.health <= 0 && slime.health != -500)
		{
			if (slime.deathTimer > 0)
			{
				AnimatorPlay(animator, "slime:death");
				slime.deathTimer--;
			}
			else
			{
				slime.health = -500;
				Vector2 spritePosition = s.position;
				Engine::Registry().remove_all(entity);

				auto& newSprite = Engine::Registry().emplace<Sprite>(entity);
				AssignSprite(newSprite, "Puddle");
				newSprite.size = { 80, 80 };
				newSprite.position = { spritePosition, 20.f };

				//When the blob dies, it creates 8 smaller blob projectiles that go in different dirrections
				for (int i = 0; i < 8; i++)
				{
					switch (i)
					{
					case 0: CreateBullet(newSprite.position, { rand(), -rand() }, Unit::slime, "Blob"); break;
					case 1: CreateBullet(newSprite.position, { -rand(), rand() }, Unit::slime, "Blob"); break;
					case 2: CreateBullet(newSprite.position, { -rand(), -rand() }, Unit::slime, "Blob"); break;
					case 3: CreateBullet(newSprite.position, { rand(), rand() }, Unit::slime, "Blob"); break;
					case 4: CreateBullet(newSprite.position, { rand(), -rand() }, Unit::slime, "Blob"); break;
					case 5: CreateBullet(newSprite.position, { rand(), rand() }, Unit::slime, "Blob"); break;
					case 6: CreateBullet(newSprite.position, { -rand(), rand() }, Unit::slime, "Blob"); break;
					case 7: CreateBullet(newSprite.position, { -rand(), -rand() }, Unit::slime, "Blob"); break;
					}
				}
			}       

		
			col.colided = false;
		}
			

		else if (slime.health > 0)
		{
			if (col.colided && !Engine::Registry().has<Bullet>(col.colidedWith))
			{
				if(Engine::Registry().valid(col.colidedWith) && Engine::Registry().has<Transform>(col.colidedWith))
				{
					SimpleCollision& collision = Engine::Registry().get<SimpleCollision>(col.colidedWith);
					Transform& transform = Engine::Registry().get<Transform>(col.colidedWith);

					Vector2 collisionSides = col.GetCollisionSides(t.position, collision, transform.position);

					do
					{
						Float32 dt = Engine::DeltaTime();
						if (collisionSides.x > 0)
						{
							t.position.x -= slime.speed * dt;
						}

						if (collisionSides.y > 0)
						{
							t.position.y -= slime.speed * dt;
						}
						if (collisionSides.x < 0)
						{
							t.position.x += slime.speed * dt;
						}

						if (collisionSides.y < 0)
						{
							t.position.y += slime.speed * dt;
						}
					} while (col.IsCollided(t.position, collision, transform.position));
				}
				col.colided = false;
			}

			else
			{
				Vector2 slimeSpeed = speedForFollow(t.position, playerPosition, slime.speed);

				t.position.x += slimeSpeed.x * Engine::DeltaTime();
				t.position.y += slimeSpeed.y * Engine::DeltaTime();

				if (col.colided && Engine::Registry().has<Bullet>(col.colidedWith))
				{
					auto& bullet = Engine::Registry().get<Bullet>(col.colidedWith);
					if (bullet.ownership == player)
						slime.health -= bullet.damage;
					col.colided = false;
				}
			}
		}
	}
}

Vector2 lab::speedForFollow(Vector2 position_, Vector2 target_, Float32 speed_)
{
	Vector2 result;

	Vector2 directions = { 1, 1 };
	if (target_.x < position_.x)
		directions.x = -1;
	else if (target_.x == position_.x)
		directions.x = 0;

	if ((-1 * target_.y) < position_.y)
		directions.y = -1;
	else if ((-1 * target_.y) == position_.y)
		directions.y = 0;

	Float32 distanceX = target_.x - position_.x;
	Float32 distanceY = (target_.y * -1) - position_.y;
	Float32 ratio = distanceY / distanceX;
	ratio *= (ratio > 0) ? 1 : -1;

	Float32 speedXIntensity = sqrt((speed_ * speed_) / (1 + ratio * ratio));
	result.x = speedXIntensity * directions.x;
	result.y = speedXIntensity * ratio * directions.y;

	return result;
}
