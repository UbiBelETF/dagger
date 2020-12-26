#include "rocket.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/tank_controller.h"
#include "gameplay/tank_warfare/collectibles.h"

using namespace tank_warfare;

void RocketSystem::Run()
{
	auto view = Engine::Registry().view<Transform, SimpleCollision, Rocket, Animator>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& t = view.get<Transform>(entity);
		auto& rocket = view.get<Rocket>(entity);
		auto& anim = view.get<Animator>(entity);

		if (col.colided)
		{
			if (Engine::Registry().has<TankCharacter>(col.colidedWith))
			{
				auto& tank = Engine::Registry().get<TankCharacter>(col.colidedWith);
				auto& tra = Engine::Registry().get<Transform>(col.colidedWith);
				if (rocket.id == tank.id)
				{
					t.position.x += rocket.speed.x * Engine::DeltaTime();
					t.position.y += rocket.speed.y * Engine::DeltaTime();
					t.position.z = tra.position.z + 1;
				}
				else
				{
					if (tank.shield > 0) 
					{
						tank.shield -= rocket.damage;
						rocket.damage = 0;
					}
					if (tank.shield < 0)
					{
						tank.health += tank.shield;
						tank.shield = 0;
						rocket.damage = 0;
					}
					tank.health -= rocket.damage;
					rocket.damage = 0;
					rocket.explode = true;
					AnimatorPlay(anim, "explosion:small");
					if (anim.currentFrame == AnimatorNumberOfFrames(anim))
					{
						rocket.toBeDestroyed = true;
					}
				}
			}
			else
			if(Engine::Registry().has<Collectible>(col.colidedWith))
			{
				t.position.x += rocket.speed.x * Engine::DeltaTime();
				t.position.y += rocket.speed.y * Engine::DeltaTime();
			}
			else
			{
				auto& c = Engine::Registry().get<SimpleCollision>(col.colidedWith);
				auto& tr = Engine::Registry().get<Transform>(col.colidedWith);
				Vector2 collisionSides = c.GetCollisionSides(tr.position, c, t.position);

				if (collisionSides.y < 0)
				{
					t.position.z = tr.position.z - 1;
				}

				AnimatorPlay(anim, "explosion:small");
				if (anim.currentFrame == AnimatorNumberOfFrames(anim))
				{
					rocket.toBeDestroyed = true;
				}
			}
			col.colided = false;
		}
		else
		{
			if (rocket.explode)
			{
				AnimatorPlay(anim, "explosion:small");
				if (anim.currentFrame == AnimatorNumberOfFrames(anim))
				{
					rocket.toBeDestroyed = true;
				}
			}
			else
			{
				t.position.x += rocket.speed.x * Engine::DeltaTime();
				t.position.y += rocket.speed.y * Engine::DeltaTime();
				t.position.z = 2;
			}
		}
	}
}

void RocketSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&RocketSystem::OnEndOfFrame>(this);
}

void RocketSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&RocketSystem::OnEndOfFrame>(this);
}

void RocketSystem::OnEndOfFrame()
{
	auto view = Engine::Registry().view<Rocket>();

	for (auto entity : view)
	{
		auto& rocket = view.get<Rocket>(entity);
		if (rocket.toBeDestroyed)
		{
			Engine::Registry().destroy(entity);
		}
	}
}

void tank_warfare::CreateRocket(Vector2 position_, Vector2 speed_, int id_, ETankOrientation to_, bool isUpgraded_)
{
	auto& reg = Engine::Registry();
	auto entity = reg.create();
	
	auto& sprite = reg.emplace<Sprite>(entity);

	switch (to_)
	{
	case ETankOrientation::TankLeft: AssignSprite(sprite, "jovanovici:tank:rocket"); break;
	case ETankOrientation::TankRight: AssignSprite(sprite, "jovanovici:tank:rocket"); sprite.scale.x *= -1; break;
	case ETankOrientation::TankUp: AssignSprite(sprite, "jovanovici:tank:rocketU"); break;
	case ETankOrientation::TankDown: AssignSprite(sprite, "jovanovici:tank:rocketD"); break;
	}

	auto& anim = reg.emplace<Animator>(entity);

	auto& transform = reg.emplace<Transform>(entity);
	transform.position.x = position_.x;
	transform.position.y = position_.y;
	if (isUpgraded_ == false)
	{
		transform.position.y += 3;
	}
	transform.position.z = 4;

	auto& col = reg.emplace<SimpleCollision>(entity);
	if (isUpgraded_ == false)
	{
		col.size = { 11, 7 };
		sprite.size /= 1.5;
	}
	else
	{
		col.size = { 15, 10 };
	}

	auto& rocket = reg.emplace<Rocket>(entity);
	rocket.speed = speed_;
	rocket.id = id_;
}
