#pragma once
#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct BrawlerCharacter
	{
		int speed{ 1 };
		bool jump{ false };
		bool attack{ false };
		Float32 attack_time{ 0 };
		bool run{ false };
		Float32 jump_time{ 0 };
		bool fall{ false };
		Float32 fall_time{ 0 };
	};

	class BrawlerControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Controller System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};
}
