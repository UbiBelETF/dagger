#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
	struct ControllerMapping
	{
		EDaggerKeyboard leftKey = EDaggerKeyboard::KeyA;
		EDaggerKeyboard rightKey = EDaggerKeyboard::KeyD;
		EDaggerKeyboard upKey = EDaggerKeyboard::KeyW;
		EDaggerKeyboard downKey = EDaggerKeyboard::KeyS;

		Vector2 input{ 0, 0 };
	};

	struct TeamGameCharacter
	{
		int speed{ 1 };
	};

	class TeamGameControllerSystem
		: public System
	{
		void OnInitialize(Registry& registry_, Entity entity_);

	public:
		String SystemName() override {
			return "Team Game Character Controller System";
		}

		void SpinUp() override;
		void Run() override;
		void WindDown() override;

	private:

		void OnKeyboardEvent(KeyboardEvent kEvent_);
	};


}
