#pragma once

#include "core/graphics/window.h"
#include "core/system.h"
#include "core/core.h"

#include <chrono>

using namespace dagger;

class DiagnosticSystem 
	: public System
	, public Subscriber<GUIRender, Frame>
{
	using TimePoint = std::chrono::time_point< std::chrono::steady_clock>;
	using Duration = std::chrono::duration<double>;

	uint64_t m_LastFrameCounter;
	uint64_t m_FrameCounter;
	TimePoint m_LastFrameTime;
	Duration m_DeltaTime;

	void Tick();
	void RenderGUI();

public:
	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};

