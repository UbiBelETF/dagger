#pragma once

#include "system.h"
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

	void SpinUp(entt::dispatcher& events_) override
	{
		events_.sink<dagger::InputEvent>().connect<&KeyLogSystem::CheckInput>(this);
	}

	void Run(entt::dispatcher& events_) override
	{
	}

	void WindDown(entt::dispatcher& events_) override
	{
		events_.sink<dagger::InputEvent>().disconnect<&KeyLogSystem::CheckInput>(this);
	}
};

