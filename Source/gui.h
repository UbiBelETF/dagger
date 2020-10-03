#pragma once

#include "engine.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "structures.h"

struct GUISystem : public System
{
	void SpinUp(Engine&) override 
	{
		auto renderConfig = Engine::Cache<RenderConfig>();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(renderConfig.m_Window, false);

		Engine::Dispatch().sink<PreRender>().connect<&GUISystem::OnPreRender>(this);
		Engine::Dispatch().sink<PostRender>().connect<&GUISystem::OnPostRender>(this);
	}

	void OnPreRender()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		Engine::Dispatch().trigger<GUIRender>();
		ImGui::Render();
	}

	void OnPostRender()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void WindDown(Engine&) override
	{
		Engine::Dispatch().sink<PreRender>().disconnect<&GUISystem::OnPreRender>(this);
		Engine::Dispatch().sink<PostRender>().disconnect<&GUISystem::OnPostRender>(this);
		ImGui::DestroyContext();
	}
};