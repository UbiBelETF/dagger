#pragma once

#include "core/system.h"
#include "core/core.h"
using namespace dagger;

namespace plight {



	struct Weapon {
		Float32 currentTimer = 0.f;
		Float32 animTimer = 1.f;
		Float32 weaponDamage = 1.f;

		bool attacking = false;

		bool animPlaying{ false };
		Entity holder;

	};



	class MeleeSystem
		: public System
	{
		inline String SystemName() { return "Melee System"; }

		void SpinUp() override;
		void Run() override;
		void WindDown() override;



	public:


	private:
		void OnEndOfFrame();

	};

}

