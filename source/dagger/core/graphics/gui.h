#pragma once

#include "core/system.h"
#include "core/core.h"
#include "window.h"

using namespace dagger;

class GUISystem 
	: public System
	, public Subscriber<PreRender, ToolRender>
	, public Publisher<GUIRender>
{
	void OnPreRender();
	void OnToolRender();

public:
	void SpinUp() override;
	void WindDown() override;
};