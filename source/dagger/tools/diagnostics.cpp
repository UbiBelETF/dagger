
#include "tools/diagnostics.h"
#include "tools/plotvar.h"
#include "core/engine.h"
#include "core/input/inputs.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

void DiagnosticSystem::Tick()
{
	m_FrameCounter++;
}

void DiagnosticSystem::RenderGUI()
{
	ImGui::SetNextWindowSize(ImVec2(200, 60), ImGuiCond_FirstUseEver);
	ImGui::Begin("Diagnostics");
	ImGui::PlotVar("FPS", (Float32)m_LastFrameCounter);
	ImGui::Separator();

	{
		auto& cursor = dagger::Input::CursorPositionInWindow();
		ImGui::Text("Window: %f %f", cursor.x, cursor.y);
	}

	{
		auto& cursor = dagger::Input::CursorPositionInScreen();
		ImGui::Text("Screen: %f %f", cursor.x, cursor.y);
	}

	{
		auto& cursor = dagger::Input::CursorPositionInWorld();
		ImGui::Text("World: %f %f", cursor.x, cursor.y);
	}

	ImGui::End();
}

#if defined(MEASURE_SYSTEMS)
void DiagnosticSystem::ReceiveSystemStats(SystemRunStats stats_)
{
	auto length = stats_.length.count();
	m_SystemTimeCounter += length;

	if (!m_SystemStats.contains(stats_.name))
		m_SystemStats[stats_.name] = length;
	else
		m_SystemStats[stats_.name] = (m_SystemStats[stats_.name] + length) / 2.0f;
}
#endif//defined(MEASURE_SYSTEMS)

void DiagnosticSystem::SpinUp()
{
#if defined(MEASURE_SYSTEMS)
	Engine::Dispatcher().sink<SystemRunStats>().connect<&DiagnosticSystem::ReceiveSystemStats>(this);
#endif//defined(MEASURE_SYSTEMS)
	Engine::Dispatcher().sink<GUIRender>().connect<&DiagnosticSystem::RenderGUI>(this);
	Engine::Dispatcher().sink<NextFrame>().connect<&DiagnosticSystem::Tick>(this);
}

void DiagnosticSystem::Run()
{
	m_DeltaSum += Engine::DeltaTime();
	if (m_DeltaSum >= 1.0)
	{
		m_LastFrameCounter = m_FrameCounter;
		Logger::trace("Frame: {}, FPS: {}", Engine::FrameCount(), m_LastFrameCounter);
		m_FrameCounter = 0;

#if defined(MEASURE_SYSTEMS)
		Logger::trace("Per-system measurements #{}", Engine::FrameCount());
		for (auto& [key, value] : m_SystemStats)
		{
			Logger::trace("  {:<30}:\t\t{:>10}", key, value);
		}
		Logger::trace("Systemic time: {:>30}", m_SystemTimeCounter / m_LastFrameCounter);

		m_SystemTimeCounter = 0.0;
#endif//defined(MEASURE_SYSTEMS)

		m_DeltaSum = 0.0;
	}
}

void DiagnosticSystem::WindDown()
{
#if defined(MEASURE_SYSTEMS)
	Engine::Dispatcher().sink<SystemRunStats>().disconnect<&DiagnosticSystem::ReceiveSystemStats>(this);
#endif//defined(MEASURE_SYSTEMS)
	Engine::Dispatcher().sink<NextFrame>().disconnect<&DiagnosticSystem::Tick>(this);
	Engine::Dispatcher().sink<GUIRender>().disconnect<&DiagnosticSystem::RenderGUI>(this);
}
