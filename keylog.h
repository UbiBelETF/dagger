#pragma once

#include "engine.h"
#include "events.h"

#include <spdlog/spdlog.h>

struct KeyLogSystem : public dagger::System
{
	void CheckInput(dagger::InputEvent event_)
	{
		if (event_.GetType() == dagger::EventType::KEY_DOWN)
			spdlog::info("Key {} pressed!", (char)event_.GetKeyboard());
		else if (event_.GetType() == dagger::EventType::KEY_UP)
			spdlog::info("Key {} released!", (char)event_.GetKeyboard());
	}

	void SpinUp(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<dagger::InputEvent>().connect<&KeyLogSystem::CheckInput>(this);
	}

	void Run(dagger::Engine&) override
	{
	}

	void WindDown(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<dagger::InputEvent>().disconnect<&KeyLogSystem::CheckInput>(this);
	}
};

