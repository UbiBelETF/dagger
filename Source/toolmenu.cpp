#include "toolmenu.h"
#include "engine.h"

void ToolMenuSystem::RenderGUI()
{
	ImGui::BeginMainMenuBar();
	Engine::Dispatch().trigger<ToolMenuRender>();
	ImGui::EndMainMenuBar();
}

void ToolMenuSystem::SpinUp(Engine& engine_)
{
	Engine::Dispatch().sink<GUIRender>().connect<&ToolMenuSystem::RenderGUI>(this);
}

void ToolMenuSystem::WindDown(Engine& engine_)
{
	Engine::Dispatch().sink<GUIRender>().disconnect<&ToolMenuSystem::RenderGUI>(this);
}
