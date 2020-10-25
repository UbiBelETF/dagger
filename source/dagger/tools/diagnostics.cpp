
#include "tools/diagnostics.h"
#include "tools/plotvar.h"
#include "core/engine.h"

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
	ImGui::End();
}

void DiagnosticSystem::SpinUp()
{
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
		m_DeltaSum = 0.0;
	}
}

void DiagnosticSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&DiagnosticSystem::Tick>(this);
	Engine::Dispatcher().sink<GUIRender>().disconnect<&DiagnosticSystem::RenderGUI>(this);
}
