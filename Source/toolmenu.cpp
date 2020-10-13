#include "toolmenu.h"
#include "engine.h"

void ToolMenuSystem::RenderGUI()
{
	ImGui::BeginMainMenuBar();
	Engine::Dispatcher().trigger<ToolMenuRender>();
	ImGui::EndMainMenuBar();
}

void ToolMenuSystem::SpinUp()
{
	Engine::Dispatcher().sink<GUIRender>().connect<&ToolMenuSystem::RenderGUI>(this);
}

void ToolMenuSystem::WindDown()
{
	Engine::Dispatcher().sink<GUIRender>().disconnect<&ToolMenuSystem::RenderGUI>(this);
}
