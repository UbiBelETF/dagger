#pragma once

#include "engine.h"
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

	void SpinUp(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<dagger::InputEvent>().connect<&EscapeSystem::CheckExit>(this);
	}

	void Run(dagger::Engine& engine_) override
	{
		if (m_Exiting)
			engine_.GetDispatcher().trigger<dagger::Exit>(dagger::Exit());
	}

	void WindDown(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<dagger::InputEvent>().disconnect<&EscapeSystem::CheckExit>(this);
	}
};

