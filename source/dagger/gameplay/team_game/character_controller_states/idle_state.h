#pragma once

#include "core/system.h"

using namespace dagger;

class IdleStateSystem : public System
{
	void OnInitialize(Registry& registry_, Entity entity_);

public:
	String SystemName() override
	{
		return "Idle State System";
	}

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
