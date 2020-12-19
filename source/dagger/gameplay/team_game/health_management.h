#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	class HealthManagementSystem : public System
	{
	public:
		inline String SystemName() override {
			return "Health Management System";
		};

		void Run() override;
	};
}
