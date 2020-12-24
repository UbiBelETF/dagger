#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Weapon {
	Float32 weaponDamage = 0.01f;

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

