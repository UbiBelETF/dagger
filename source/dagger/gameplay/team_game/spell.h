#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace ancient_defenders {

	struct SpellStats {
		Float32 dmg;

		String type;
		Float32 time;
	};

	struct Spell {
		Entity entity;

		static Entity Create(Vector3 position_, String type_);
	};

	class SpellBehaviorSystem
		: public System
	{

	public:
		inline String SystemName() { return "Spell Behavior System"; }

		void SpinUp() override;
		void WindDown() override;
		void Run() override;

	private:
		void OnEndOfFrame();
	};

}