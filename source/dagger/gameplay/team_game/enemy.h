#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace lab
{
	struct Bandit {
		Float32 speed = 20.f;
		int cooldown = 10;
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
