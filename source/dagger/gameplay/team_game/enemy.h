#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	enum enemyType { null, follower, vertical, horizontal, boss1, boss2 };

	struct Skeleton {
		enemyType type;
		Float32 speed = 10.f;
		Float32 speedX = 10.f;
		Float32 speedY = 10.f;
		int cooldown = 150;
		int maxCooldown = 150;
		int meleeCooldown = 300;
		int maxMeleeCooldown = 300;
		Float32 meleeDamage = 0.5f;
		int health = 100;
		int bossHealth = 250;
		Float32 deathTimer = 125;
		int bossCooldown = 60;
		int bossMaxCooldown = 60;
	};

	struct Slime {
		Float32 speed = 2.f;
		Float32 speedX;
		Float32 speedY;
		Float32 damage = 1;
		int health = 150;
		Float32 deathTimer = 125;
	};

	class EnemySystem
		: public System
	{
	public:
		String SystemName() override {
			return "Enemy System";
		}
		void Run() override;
	};

	Vector2 speedForFollow(Vector2 position_, Vector2 target_, Float32 speed_);
}
