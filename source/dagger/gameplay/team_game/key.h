#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Key {
	bool ispickedup;

	void SetPickedUp(bool condition_) { ispickedup = condition_; }

	bool GetPickedUp() { return ispickedup; }
};

class KeySystem : public System
{

public:
	inline String SystemName() { return "Key System"; }

	void Run() override;

};