#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	enum Unit { player, skeleton, slime, Boss };

	struct Bullet {
		Unit ownership;
		int damage = 5;
		Float32 speed = 100.f;
		Float32 speedX = 100.f;
		Float32 speedY = 100.f;
		bool destroy = false;
	};

	struct Gun {
		int ID;
	};

	class ShootingSystem
		: public System
	{
	public:
		String SystemName() override {
			return "Character Controller System";
		}
		void Run() override;
	};
	void CreateBullet(Vector2 position, Vector2 target, Unit owner, String spritename);

}