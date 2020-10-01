#pragma once

#include "engine.h"
#include "events.h"

#include <spdlog/spdlog.h>

using namespace dagger;

struct KeyLogSystem : public System
{
	void CheckInput(InputEvent event_)
	{
		if (event_.GetType() == EventType::KEY_DOWN)
			spdlog::info("Key {} pressed!", (char)event_.GetKeyboard());
		else if (event_.GetType() == EventType::KEY_UP)
			spdlog::info("Key {} released!", (char)event_.GetKeyboard());
	}

	void SpinUp(Engine& engine_) override
	{
		engine_.GetDispatcher().sink<InputEvent>().connect<&KeyLogSystem::CheckInput>(this);
	}

	void Run(Engine&) override
	{
	}

	void WindDown(Engine& engine_) override
	{
		engine_.GetDispatcher().sink<InputEvent>().disconnect<&KeyLogSystem::CheckInput>(this);
	}
};

