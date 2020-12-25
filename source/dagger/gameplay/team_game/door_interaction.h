#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Door {
	
};



class DoorSystem : public System
{
	Bool pickedup = false;
public:
	inline String SystemName() { return "Door System"; }

	void Run() override;

	void SetPickedUp(Bool value_){
		pickedup = value_;
	}

	Bool GetPickedUpp() {
	   	return pickedup;
	}

};