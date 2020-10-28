#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace example1
{
	class CharacterControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Character Controller System";
		}

		inline void SpinUp() override;
		inline void Run() override;
		inline void WindDown() override;
	};

	void SetupSystems(Engine& engine_);
	void SetupWorld(Engine& engine_);
}