#pragma once

#include "system.h"
#include "structures.h"
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