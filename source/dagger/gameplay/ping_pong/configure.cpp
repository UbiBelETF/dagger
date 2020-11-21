
#include "configure.h"
#if defined(DAGGER_DEBUG)
#include "tools/plotvar.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/window.h"

#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

Float32 ConfigureSystem::m_BallSpeedMultiplier = 1.f;
Float32 ConfigureSystem::m_GameSpeedMultiplier = 1.f;
Float32 ConfigureSystem::m_PlayerSpeedMultiplier = 1.f;
bool ConfigureSystem::m_paused = false;
  Float32 ConfigureSystem::BallSpeedMultiplier() {
	return m_BallSpeedMultiplier;
}
  Float32 ConfigureSystem::PlayerSpeedMultiplier() {
	 return m_PlayerSpeedMultiplier;
}
  Float32 ConfigureSystem::GameSpeedMultiplier() {
	 if (m_paused)return 0;
	 else return m_GameSpeedMultiplier;
}
void ConfigureSystem::RenderGUI()
{
	ImGui::SetNextWindowSize(ImVec2(200, 60), ImGuiCond_FirstUseEver);
	ImGui::Begin("Configure");
	ImGui::Separator();

	{
		ImGui::SliderFloat("Ball speed", &ConfigureSystem::m_BallSpeedMultiplier, 0.1f, 3.0f);
	}

	{
		ImGui::SliderFloat("Player speed", &ConfigureSystem::m_PlayerSpeedMultiplier, 0.1f, 3.0f);
	}

	{
		ImGui::SliderFloat("Game speed", &ConfigureSystem::m_GameSpeedMultiplier, 0.3, 5.0f);
	}

	{
		ImGui::Checkbox("Pause", &ConfigureSystem::m_paused);
	}
	ImGui::End();
}

#

void ConfigureSystem::SpinUp()
{

	Engine::Dispatcher().sink<GUIRender>().connect<&ConfigureSystem::RenderGUI>(this);
}

void ConfigureSystem::Run()
{

}

void ConfigureSystem::WindDown()
{
	Engine::Dispatcher().sink<GUIRender>().disconnect<&ConfigureSystem::RenderGUI>(this);
}
#endif

