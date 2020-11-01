
#include "window.h"
#include "core/core.h"
#include "core/engine.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

static void ErrorCallback(int error_, const char* description_)
{
	Logger::error("GLFW Error {}): {}\n", error_, description_);
	Engine::Dispatcher().trigger<Error>(Error{ description_ });
}

static void KeyCallback(GLFWwindow* window_, int key_, int scancode_, int action_, int mods_)
{
	Engine::Dispatcher().trigger<KeyboardEvent>(KeyboardEvent{ (EDaggerKeyboard)key_, (EDaggerInputState)action_, (UInt32)scancode_, (UInt32)mods_ });
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
	Engine::Dispatcher().trigger<MouseEvent>(MouseEvent{ (EDaggerMouse)(button_ + 150), (EDaggerInputState)action_, (UInt32)mods_ });
}

static void CursorCallback(GLFWwindow* window_, double x_, double y_)
{
	Engine::Dispatcher().trigger<CursorEvent>(CursorEvent{ (Float64)x_, (Float64)y_ });
}

static void WindowResizeCallback(GLFWwindow* window_, int width_, int height_)
{
	Engine::Dispatcher().trigger<WindowResized>(WindowResized{ window_, (UInt32)width_, (UInt32)height_ });
}

void WindowSystem::OnWindowResized(WindowResized resized_)
{
	auto& [window_, width_, height_] = resized_;
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();
	config->windowWidth = width_;
	config->windowHeight = height_;

	switch (camera->mode)
	{
	case ECameraMode::FixedWidth:
		{
			Float32 height = camera->size.x * (Float32)height_ / (Float32)width_;
			config->projection = glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);
		}
		break;
	case ECameraMode::FixedHeight:
		{
			Float32 width = camera->size.y * (Float32)width_ / (Float32)height_;
			config->projection = glm::ortho(-width / 2.0f, width / 2.0f, -height_ / 2.0f, height_ / 2.0f, -1.0f, 1.0f);
		}
		break;
	case ECameraMode::FixedResolution:
		{
			Float32 width = camera->size.x / 2;
			Float32 height = camera->size.y / 2;
			config->projection = glm::ortho(-width, width, -height, height, -1.0f, 1.0f);
		}
		break;
	case ECameraMode::ShowAsMuchAsPossible:
	default:
		config->projection = glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height_ / 2.0f, height_ / 2.0f, -1.0f, 1.0f);
		break;
	}

	glUniformMatrix4fv((GLuint)Shader::EUniforms::ProjectionMatrixId, 1, false, glm::value_ptr(config->projection));
}

void WindowSystem::OnShaderChanged(ShaderChangeRequest request_)
{
	Logger::info("Shader changed to {}, reuploading projection matrix.", request_.m_Shader->shaderName);
	WindowResizeCallback(m_Config.window, m_Config.windowWidth, m_Config.windowHeight);
}

void WindowSystem::OnCameraUpdated(Camera camera_)
{
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();

	bool changedProjection = false;
	bool changedView = false;

	if (camera->mode != camera_.mode)
	{
		camera->mode = camera_.mode;
		changedProjection = true;
	}

	if (camera->size != camera_.size)
	{
		camera->size = camera_.size;
		changedProjection = true;
	}

	if (camera->position != camera_.position)
	{
		camera->position = camera_.position;
		changedView = true;
	}

	if (camera->zoom != camera_.zoom)
	{
		camera->zoom = camera_.zoom;
		changedView = true;
	}

	if (changedProjection)
	{
		auto width_ = config->windowWidth;
		auto height_ = config->windowHeight;

		switch (camera->mode)
		{
		case ECameraMode::FixedWidth:
		{
			Float32 height = camera->size.x * (Float32)height_ / (Float32)width_;
			config->projection = glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height / 2.0f, height / 2.0f, -1.0f, 1.0f);
		}
		break;
		case ECameraMode::FixedHeight:
		{
			Float32 width = camera->size.y * (Float32)width_ / (Float32)height_;
			config->projection = glm::ortho(-width / 2.0f, width / 2.0f, -height_ / 2.0f, height_ / 2.0f, -1.0f, 1.0f);
		}
		break;
		case ECameraMode::FixedResolution:
		{
			Float32 width = camera->size.x / 2;
			Float32 height = camera->size.y / 2;
			config->projection = glm::ortho(-width, width, -height, height, -1.0f, 1.0f);
		}
		break;
		case ECameraMode::ShowAsMuchAsPossible:
		default:
			config->projection = glm::ortho(-width_ / 2.0f, width_ / 2.0f, -height_ / 2.0f, height_ / 2.0f, -1.0f, 1.0f);
			break;
		}

		glUniformMatrix4fv((GLuint)Shader::EUniforms::ProjectionMatrixId, 1, false, glm::value_ptr(config->projection));
	}

	glm::mat4 scaleMatrix = glm::scale(glm::vec3(camera->zoom));
	glm::mat4 translateMatrix = glm::translate(glm::vec3(camera->position, 0));
	m_Config.cameraView = translateMatrix * scaleMatrix * glm::identity<glm::mat4>();
	glUniformMatrix4fv((GLuint)Shader::EUniforms::CameraViewMatrixId, 1, false, glm::value_ptr(m_Config.cameraView));
}

void WindowSystem::SpinUp()
{
	Logger::info("Booting up renderer");

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

	if (m_Config.fullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		m_Config.windowWidth = mode->width;
		m_Config.windowHeight = mode->height;
	}

	m_Config.window = glfwCreateWindow(m_Config.windowWidth, m_Config.windowHeight, "Dagger", monitor, nullptr);

	auto* window = m_Config.window;
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

	Engine::PutDefaultResource<RenderConfig>(&m_Config);
	Engine::PutDefaultResource<Camera>(&m_Camera);

	Engine::Dispatcher().sink<ShaderChangeRequest>().connect<&WindowSystem::OnShaderChanged>(this);
	Engine::Dispatcher().sink<WindowResized>().connect<&WindowSystem::OnWindowResized>(this);
	Engine::Dispatcher().sink<Camera>().connect<&WindowSystem::OnCameraUpdated>(this);

	WindowResizeCallback(window, m_Config.windowWidth, m_Config.windowHeight);

	glm::mat4 scaleMatrix = glm::scale(glm::vec3(m_Camera.zoom));
	glm::mat4 translateMatrix = glm::translate(glm::vec3(m_Camera.position, 0));
	m_Config.cameraView = translateMatrix * scaleMatrix * glm::identity<glm::mat4>();
	glUniformMatrix4fv((GLuint)Shader::EUniforms::CameraViewMatrixId, 1, false, glm::value_ptr(m_Config.cameraView));

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, CharCallback);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void WindowSystem::Run()
{
	auto* window = m_Config.window;
	Engine::Dispatcher().trigger<PreRender>();

	glViewport(0, 0, m_Config.windowWidth, m_Config.windowHeight);
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

	glfwDestroyWindow(m_Config.window);
	glfwTerminate();

	Engine::Dispatcher().sink<ShaderChangeRequest>().disconnect<&WindowSystem::OnShaderChanged>(this);
	Engine::Dispatcher().sink<WindowResized>().disconnect<&WindowSystem::OnWindowResized>(this);
	Engine::Dispatcher().sink<Camera>().disconnect<&WindowSystem::OnCameraUpdated>(this);
}