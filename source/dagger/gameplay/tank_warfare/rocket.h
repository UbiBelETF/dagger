#pragma once

#include "core/core.h"
#include "core/system.h"

#include "gameplay/tank_warfare/tank_stats.h"

using namespace dagger;

namespace tank_warfare
{
	void CreateRocket(Vector2 position_, Vector2 speed_, int id_, ETankOrientation to_);

	struct Rocket
	{
		int damage{ 35 };
		bool toBeDestroyed{ false };
		Vector2 speed{ 0, 0 };
		int id{ 0 };
	};

	class RocketSystem
		: public System
	{
	public:
		String SystemName() override {
			return "Rocket System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;

	private:
		void OnEndOfFrame();
	};
}