
#include "window.h"
#include "core/core.h"
#include "core/engine.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

static void ErrorCallback(int error_, const char* description_)
{
	Logger::error("GLFW Error {}): {}\n", error_, description_);
	Engine::Dispatcher().trigger<Error>(Error{ description_ });
}

static void KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
{
	Engine::Dispatcher().trigger<KeyboardEvent>(KeyboardEvent{ (UInt32)key_, (UInt32)scancode_, (UInt32)action_, (UInt32)mods_ });
}

static void CharCallback(GLFWwindow* window_, unsigned int codepoint_)
{
	Engine::Dispatcher().trigger<CharEvent>(CharEvent{ (UInt8)codepoint_ });
}

static void ScrollCallback(GLFWwindow* window_, double xOffset_, double yOffset_)
{
	Engine::Dispatcher().trigger<ScrollEvent>(ScrollEvent{ (Float64)xOffset_, (Float64)yOffset_ });
}

static void MouseCallback(GLFWwindow* window_, int button_, int action_, int mods_)
{
	Engine::Dispatcher().trigger<MouseEvent>(MouseEvent{ (UInt32)button_, (UInt32)action_, (UInt32)mods_ });
}

static void CursorCallback(GLFWwindow* window_, double x_, double y_)
{
	Engine::Dispatcher().trigger<CursorEvent>(CursorEvent{ (Float64)x_, (Float64)y_ });
}

static void WindowResizeCallback(GLFWwindow* window_, int width_, int height_)
{
	auto config = reinterpret_cast<RenderConfig*>(glfwGetWindowUserPointer(window_));
	config->m_WindowWidth = width_;
	config->m_WindowHeight = height_;

    Float32 width = 900; // (Float32)width_;
    Float32 height = width * (Float32)height_ / (Float32)width_; // (Float32)height_;

	config->m_Projection = glm::ortho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);
	glUniformMatrix4fv((GLuint)Shader::Uniforms::ProjectionMatrixId,
		1, false, glm::value_ptr(config->m_Projection));
	Engine::Dispatcher().trigger<WindowResizedEvent>(WindowResizedEvent{ (UInt32)width_, (UInt32)height_ });
}

void WindowSystem::OnShaderChanged(ShaderChangeRequest request_)
{
	Logger::info("Shader changed to {}, reuploading projection matrix.", request_.m_Shader->shaderName);
	WindowResizeCallback(m_Config.m_Window, m_Config.m_WindowWidth, m_Config.m_WindowHeight);
}

void WindowSystem::SpinUp()
{
	Logger::info("Booting up renderer");
	Engine::Dispatcher().sink<ShaderChangeRequest>().connect<&WindowSystem::OnShaderChanged>(this);

	auto& events = Engine::Dispatcher();

	if (!glfwInit())
	{
		events.trigger<Error>(Error{ "GLFW failed to initialize." });
		return;
	}

	glfwSetErrorCallback(ErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = nullptr;

	if (m_Config.m_Fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		m_Config.m_WindowWidth = mode->width;
		m_Config.m_WindowHeight = mode->height;
	}

	m_Config.m_Window = glfwCreateWindow(m_Config.m_WindowWidth, m_Config.m_WindowHeight, "Dagger", monitor, nullptr);

	auto* window = m_Config.m_Window;
	glfwSetWindowUserPointer(window, (void*) &m_Config);

	if (window == nullptr)
	{
		events.trigger<Error>(Error{ "GLFW window failed to create." });
		return;
	}

	glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		events.trigger<Error>(Error{ "Couldn't initialize GLAD" });
		return;
	}

	WindowResizeCallback(window, m_Config.m_WindowWidth, m_Config.m_WindowHeight);

	Engine::Res<RenderConfig>()["Render Config"] = &m_Config;

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, CharCallback);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void WindowSystem::Run()
{
	auto* window = m_Config.m_Window;
	Engine::Dispatcher().trigger<PreRender>();

	glViewport(0, 0, m_Config.m_WindowWidth, m_Config.m_WindowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.39f, 0.58f, 0.92f, 1.0f);

	Engine::Dispatcher().trigger<Render>();
	Engine::Dispatcher().trigger<ToolRender>();

	glfwSwapBuffers(window);

	Engine::Dispatcher().trigger<PostRender>();

	glfwPollEvents();

	if (glfwWindowShouldClose(window))
		Engine::Dispatcher().trigger<Exit>();
}

void WindowSystem::WindDown()
{
	Logger::info("Winding down renderer");

	glfwDestroyWindow(m_Config.m_Window);
	glfwTerminate();
}