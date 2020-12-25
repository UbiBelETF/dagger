#pragma once

#include "mage.h"

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

	struct Enemy {
		Float32 speed;
		Float32 meleeDmg;

		SInt32 postition = (SInt32) WalkingPath::numberOfPoints - 1;
        Vector2 offset;
		Vector2 direction = { -1, 0 };
		String type;

		EAction currentAction = EAction::Moving;
	};

	struct Golem {
		Entity entity;

		static Entity Create(String type_);
	};

	class EnemyBehaviorSystem
		: public System
	{

	public:
		inline String SystemName() { return "Enemy Behavior System"; }

		void SpinUp() override;
		void WindDown() override;
		void Run() override;

	private:
		void OnEndOfFrame();
	};

}