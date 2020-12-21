#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Door {
	bool data;
};



class DoorSystem : public System
{

public:
	inline String SystemName() { return "Door System"; }

	void Run() override;

};