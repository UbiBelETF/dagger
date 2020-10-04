#pragma once

#include "system.h"
#include "structures.h"
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
	void SpinUp(Engine& engine_) override;
	void WindDown(Engine& engine_) override;
};