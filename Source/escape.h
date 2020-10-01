#pragma once

#include "engine.h"
#include "events.h"

#include <spdlog/spdlog.h>

using namespace dagger;

struct EscapeSystem : public System
{
	bool m_Exiting = false;

	void CheckExit(InputEvent event_)
	{
		if (event_.GetType() == EventType::KEY_DOWN && event_.GetKeyboard() == Keyboard::ESCAPE)
			m_Exiting = true;
	}

	void SpinUp(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<InputEvent>().connect<&EscapeSystem::CheckExit>(this);
	}

	void Run(dagger::Engine& engine_) override
	{
		if (m_Exiting)
			engine_.GetDispatcher().trigger<Exit>(Exit());
	}

	void WindDown(dagger::Engine& engine_) override
	{
		engine_.GetDispatcher().sink<InputEvent>().disconnect<&EscapeSystem::CheckExit>(this);
	}
};

