#pragma once

#include "core/graphics/window.h"
#include "core/system.h"
#include "core/core.h"

#include <chrono>

using namespace dagger;

class DiagnosticSystem 
	: public System
	, public Subscriber<GUIRender, NextFrame>
{
	UInt64 m_LastFrameCounter;
	UInt64 m_FrameCounter;
	Float64 m_DeltaSum;

	void Tick();
	void RenderGUI();

public:
	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};

