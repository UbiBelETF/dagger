#pragma once

#include "system.h"
#include "events.h"

#include <spdlog/spdlog.h>

struct EscapeSystem : public dagger::System
{
	bool m_Exiting = false;

	void CheckExit(dagger::InputEvent event_)
	{
		if (event_.GetType() == dagger::EventType::KEY_DOWN && event_.GetKeyboard() == dagger::Keyboard::ESCAPE)
			m_Exiting = true;
	}

	void SpinUp(entt::dispatcher& events_) override
	{
		events_.sink<dagger::InputEvent>().connect<&EscapeSystem::CheckExit>(this);
	}

	void Run(entt::dispatcher& events_) override
	{
		if (m_Exiting)
			events_.trigger<dagger::Exit>(dagger::Exit());
	}

	void WindDown(entt::dispatcher& events_) override
	{
		events_.sink<dagger::InputEvent>().disconnect<&EscapeSystem::CheckExit>(this);
	}
};

