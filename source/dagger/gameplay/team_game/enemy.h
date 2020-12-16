#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	enum Type {follower, vertical, horizontal};

	struct Bandit {
		Type ID;
		Float32 speed = 10.f;
		Float32 speedX = 10.f;
		Float32 speedY = 10.f;
		int cooldown = 200;
		int maxCooldown = 200;
		int health = 100;
		Float32 timeWounded = 100;
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
}
