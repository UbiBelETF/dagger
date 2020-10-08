#include "gui.h"
#include "engine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace dagger;

void GUISystem::SpinUp(Engine&)
{
	auto renderConfig = Engine::Cache<RenderConfig>("Render Config");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(renderConfig.m_Window, false);

	Engine::Dispatch().sink<PreRender>().connect<&GUISystem::OnPreRender>(this);
	Engine::Dispatch().sink<ToolRender>().connect<&GUISystem::OnToolRender>(this);
}

void GUISystem::OnPreRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	Engine::Dispatch().trigger<GUIRender>();
	ImGui::Render();
}

void GUISystem::OnToolRender()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUISystem::WindDown(Engine&)
{
	Engine::Dispatch().sink<PreRender>().disconnect<&GUISystem::OnPreRender>(this);
	Engine::Dispatch().sink<ToolRender>().disconnect<&GUISystem::OnToolRender>(this);
	ImGui::DestroyContext();
}