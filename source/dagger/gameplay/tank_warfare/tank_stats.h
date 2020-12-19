#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"

using namespace dagger;

namespace tank_warfare
{
	void CreateTankCharacter(Vector3 pos_, String input_);

	enum struct ETankOrientation
	{
		TankLeft,
		TankRight,
		TankUp,
		TankDown,
		TankDestroyed
	};

	enum struct EStatType
	{
		Health,
		Shield,
		Life
	};

	struct TankCharacter
	{
		int id{ rand() };
		int speed{ 50 };
		bool toBeDestroyed{ false };
		ETankOrientation lastOrientation{ ETankOrientation::TankLeft };
		int coins{ 0 };
		int fireRate{ 1 };
		int maxFireRate{ 2 };
		Float32 reloadTime{ 1 };
		Float32 powerDuration{ 0 };
		int lives{ 0 };
		Float32 health{ 100 };
		Float32 maxHealth{ 100 };
		Float32 shield{ 0 };
		Float32 maxShield{ 100 };

		bool triggerHealth{ false };
		bool triggerShield{ false };
	};

	struct TankStats
	{
		int id;
		int lives{ 0 };
		Float32 health{ 100 };
		Float32 shield{ 0 };
		Float32 barWidth{ 0 };
		EStatType type{ EStatType::Health };
		bool toBeDestroyed{ false };
	};

	class TankStatsSystem
		: public System
	{

	public:
		String SystemName() override {
			return "Tank Stats System";
		}

		Float32 m_Print{ 0 };

		void SpinUp() override;
		void Run() override;
		void WindDown() override;

	private:
		void OnEndOfFrame();
	};
}