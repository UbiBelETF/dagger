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
			if (Engine::Registry().has<Rocket>(col.colidedWith))
			{
				AnimatorPlay(anim, "explosion:small");
				if (anim.currentFrame == AnimatorNumberOfFrames(anim))
				{
					rocket.toBeDestroyed = true;
				}
			} 
			else
			if (Engine::Registry().has<TankCharacter>(col.colidedWith))
			{
				auto& tank = Engine::Registry().get<TankCharacter>(col.colidedWith);
				if (rocket.id == tank.id)
				{
					t.position.x += rocket.speed.x * Engine::DeltaTime();
					t.position.y += rocket.speed.y * Engine::DeltaTime();
					t.position.z = 4;
				}
				else
				{
					tank.health -= rocket.damage;
					rocket.damage = 0;
					AnimatorPlay(anim, "explosion:small");
					if (anim.currentFrame == AnimatorNumberOfFrames(anim))
					{
						rocket.toBeDestroyed = true;
					}
				}
			}
			col.colided = false;
		}
		else
		{
			t.position.x += rocket.speed.x * Engine::DeltaTime();
			t.position.y += rocket.speed.y * Engine::DeltaTime();
			t.position.z = 2;
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

void tank_warfare::CreateRocket(Vector2 position_, Vector2 speed_, int id_, ETankOrientation to_)
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
	transform.position.z = 4;

	auto& col = reg.emplace<SimpleCollision>(entity);
	col.size = { 15, 10 };

	auto& rocket = reg.emplace<Rocket>(entity);
	rocket.speed = speed_;
	rocket.id = id_;
}
