#pragma once
#if defined(DAGGER_DEBUG)
#include "core/graphics/window.h"
#include "core/system.h"
#include "core/core.h"

#include <chrono>
using namespace dagger;


class ConfigureSystem : public System
	, public Subscriber<GUIRender, NextFrame>
{
	static Float32 m_BallSpeedMultiplier;
	static Float32 m_PlayerSpeedMultiplier;
	static Float32 m_GameSpeedMultiplier;
	static bool    m_paused ;
	void RenderGUI();
public:
	inline String SystemName() { return "Configure System"; }
	static  Float32 BallSpeedMultiplier();
	static  Float32 PlayerSpeedMultiplier();
	static  Float32 GameSpeedMultiplier();
	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
#endif