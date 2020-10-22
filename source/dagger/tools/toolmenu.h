#pragma once

#include <imgui/imgui.h>

#include "core/system.h"
#include "core/core.h"
#include "core/graphics/window.h"

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