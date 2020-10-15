#include "gui.h"
#include "engine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace dagger;

void GUISystem::SpinUp()
{
	auto renderConfig = Engine::Res<RenderConfig>()["Render Config"];

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplOpenGL3_Init();
	ImguiInputWrapper::ImGui_ImplGlfw_InitForOpenGL(renderConfig->m_Window, true);

	Engine::Dispatcher().sink<PreRender>().connect<&GUISystem::OnPreRender>(this);
	Engine::Dispatcher().sink<ToolRender>().connect<&GUISystem::OnToolRender>(this);
}

void GUISystem::OnPreRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImguiInputWrapper::ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	Engine::Dispatcher().trigger<GUIRender>();
	ImGui::Render();
}

void GUISystem::OnToolRender()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUISystem::WindDown()
{
	Engine::Dispatcher().sink<PreRender>().disconnect<&GUISystem::OnPreRender>(this);
	Engine::Dispatcher().sink<ToolRender>().disconnect<&GUISystem::OnToolRender>(this);
	ImGui::DestroyContext();
}