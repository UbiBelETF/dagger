#pragma once

#include "engine.h"
#include "structures.h"
#include "imgui/imgui.h"
#include <spdlog/spdlog.h>
#include <atomic>
#include <chrono>

using namespace dagger;

struct FPSSystem : public System
{
	using TimePoint = std::chrono::time_point< std::chrono::steady_clock>;
	using Duration = std::chrono::duration<double>;

	uint64_t m_LastFrameCounter;
	uint64_t m_FrameCounter;
	TimePoint m_LastFrameTime;
	Duration m_DeltaTime;

	void Tick()
	{
		static TimePoint nextTime;
		nextTime = std::chrono::steady_clock::now();
		m_DeltaTime += (nextTime - m_LastFrameTime);
		m_LastFrameTime = nextTime;
		m_FrameCounter++;
	}

	void RenderGUI()
	{
		ImGui::SetNextWindowSize(ImVec2(120, 60));
		ImGui::SetNextWindowPos(ImVec2(10, 10));
		ImGui::Begin("Frame Counter");
		ImGui::LabelText("", "FPS: %d", m_LastFrameCounter);
		ImGui::End();
	}

	void SpinUp(Engine& engine_) override
	{
		Engine::Dispatch().sink<GUIRender>().connect<&FPSSystem::RenderGUI>(this);
		Engine::Dispatch().sink<Frame>().connect<&FPSSystem::Tick>(this);
	}

	void Run(Engine&) override
	{
		auto delta = m_DeltaTime.count();
		if (delta >= 1.0)
		{
			m_LastFrameCounter = m_FrameCounter;
			m_FrameCounter = 0;
			m_DeltaTime = Duration::zero();
		}
	}

	void WindDown(Engine& engine_) override
	{
		Engine::Dispatch().sink<Frame>().disconnect<&FPSSystem::Tick>(this);
		Engine::Dispatch().sink<GUIRender>().disconnect<&FPSSystem::RenderGUI>(this);
	}
};

