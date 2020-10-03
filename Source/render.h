#pragma once

#include "engine.h"
#include "structures.h"
#include <spdlog/spdlog.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include <deque>
#include <sstream>
 
using namespace dagger;

static void ErrorCallback(int error_, const char* description_)
{
	spdlog::error("GLFW Error ({}): {}\n", error_, description_);
	Engine::Dispatch().trigger<Error>(Error(description_));
}

static void KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
{
	Engine::Dispatch().trigger<KeyboardEvent>(KeyboardEvent{ key_, scancode_, action_, mods_ });
}

static void MouseCallback(GLFWwindow* window_, int button_, int action_, int mods_)
{
	Engine::Dispatch().trigger<MouseEvent>(MouseEvent{ button_, action_, mods_ });
}

static void CursorCallback(GLFWwindow* window_, double x_, double y_)
{
	Engine::Dispatch().trigger<CursorEvent>(CursorEvent{ x_, y_ });
}

struct RenderSystem : public System
{
	RenderConfig m_Config;

	RenderSystem(std::uint32_t width_ = 640, std::uint32_t height_ = 480)
		: m_Config{ width_, height_ }
	{}

	~RenderSystem() = default;
	RenderSystem(const RenderSystem&) = delete;

	void SpinUp(Engine& engine_) override
	{
		spdlog::info("Booting up renderer");
		auto& events = Engine::Dispatch();

		if (!glfwInit())
		{
			events.trigger<Error>(Error("GLFW failed to initialize."));
			return;
		}

		glfwSetErrorCallback(ErrorCallback);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Config.m_Window = glfwCreateWindow(m_Config.m_WindowWidth, m_Config.m_WindowHeight, "Dagger", nullptr, nullptr);

		auto* window = m_Config.m_Window;
		if (window == nullptr)
		{
			events.trigger<Error>(Error("GLFW window failed to create."));
			return;
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			events.trigger<Error>(Error("Couldn't initialize GLAD"));;
			return;
		}

		Engine::Cache<RenderConfig>() = m_Config;

		glfwSetKeyCallback(window, KeyCallback);
		glfwSetMouseButtonCallback(window, MouseCallback);
		glfwSetCursorPosCallback(window, CursorCallback);
	}
	
	void Run(Engine& engine_) override
	{
		auto* window = m_Config.m_Window;
		Engine::Dispatch().trigger<PreRender>();

		glViewport(0, 0, m_Config.m_WindowWidth, m_Config.m_WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.39f, 0.58f, 0.92f, 1.0f);

		Engine::Dispatch().trigger<PostRender>();

		glfwSwapBuffers(window);

		glfwPollEvents();

		if (glfwWindowShouldClose(window))
			Engine::Dispatch().trigger<Exit>();
	}

	void WindDown(Engine& engine_) override
	{
		spdlog::info("Winding down renderer");

		glfwDestroyWindow(m_Config.m_Window);
		glfwTerminate();
	}
};