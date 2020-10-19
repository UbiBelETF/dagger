#pragma once

#include "system.h"
#include "core.h"
#include "imgui.h"
#include "window.h"

using namespace dagger;

struct ToolMenuRender {};

struct ToolMenuSystem 
	: public System
	, public Subscriber<GUIRender>
	, public Publisher<ToolMenuRender>
{
	void RenderGUI();
	void SpinUp() override;
	void WindDown() override;
};