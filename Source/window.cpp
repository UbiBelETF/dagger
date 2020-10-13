
#include "window.h"
#include "engine.h"

static void ErrorCallback(int error_, const char* description_)
{
	spdlog::error("GLFW Error {}): {}\n", error_, description_);
	Engine::Dispatcher().trigger<Error>(Error(description_));
}

static void KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
{
	Engine::Dispatcher().trigger<KeyboardEvent>(KeyboardEvent{ key_, scancode_, action_, mods_ });
}

static void MouseCallback(GLFWwindow* window_, int button_, int action_, int mods_)
{
	Engine::Dispatcher().trigger<MouseEvent>(MouseEvent{ button_, action_, mods_ });
}

static void CursorCallback(GLFWwindow* window_, double x_, double y_)
{
	Engine::Dispatcher().trigger<CursorEvent>(CursorEvent{ x_, y_ });
}

void WindowSystem::SpinUp()
{
	spdlog::info("Booting up renderer");
	auto& events = Engine::Dispatcher();

	if (!glfwInit())
	{
		events.trigger<Error>(Error("GLFW failed to initialize."));
		return;
	}

	glfwSetErrorCallback(ErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Config.m_Window = glfwCreateWindow(m_Config.m_WindowWidth, m_Config.m_WindowHeight, "Dagger", nullptr, nullptr);

	auto* window = m_Config.m_Window;
	if (window == nullptr)
	{
		events.trigger<Error>(Error("GLFW window failed to create."));
		return;
	}

	glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		events.trigger<Error>(Error("Couldn't initialize GLAD"));;
		return;
	}

	Engine::Res<RenderConfig>()["Render Config"] = &m_Config;

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorCallback);

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
	spdlog::info("Winding down renderer");

	glfwDestroyWindow(m_Config.m_Window);
	glfwTerminate();
}