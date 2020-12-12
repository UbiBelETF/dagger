#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	struct Bandit {
		UInt16 ID;
		Float32 speed = 20.f;
		int cooldown = 200;
		int maxCooldown = 200;
		int health = 100;
		Float32 timeWounded = 100;
		static UInt16 alive;
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
