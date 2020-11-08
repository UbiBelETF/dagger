
#include "window.h"
#include "core/core.h"
#include "core/engine.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <cstring>

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

void WindowSystem::UpdateViewProjectionMatrix()
{
	glUniformMatrix4fv((GLuint)m_Matrices.viewportMatrixId, 1, false, glm::value_ptr(m_Config.viewport));
	glUniformMatrix4fv((GLuint)m_Matrices.projectionMatrixId, 1, false, glm::value_ptr(m_Config.projection));
}

void WindowSystem::UpdateViewProjectionMatrix(RenderConfig& config_, Camera& camera_)
{
	SetViewProjectionMatrix(config_, camera_, config_.lastSize.x, config_.lastSize.y);
}

void WindowSystem::SetViewProjectionMatrix(RenderConfig& config_, Camera& camera_, Float32 width_, Float32 height_)
{
	config_.lastSize = Vector2{ width_, height_ };
	switch (camera_.mode)
	{
	case ECameraMode::FixedResolution:
	{
		Float32 halfWidth = camera_.size.x / 2;
		Float32 halfHeight = camera_.size.y / 2;
		config_.projection = glm::ortho(0.0f, (Float32)camera_.size.x, 0.0f, (Float32)camera_.size.y, 0.0f, 100.0f);
		config_.viewport = glm::translate(glm::vec3(halfWidth, halfHeight, 0.0f));
		config_.viewOffset = Vector2{ halfWidth, halfHeight };
		config_.viewBounds = Vector4{ 0, 0, camera_.size.x, camera_.size.y };
	}
	break;
	case ECameraMode::ShowAsMuchAsPossible:
	default:
		config_.projection = glm::ortho(0.0f, width_, 0.0f, height_, 0.0f, 100.0f);
		config_.viewport = glm::translate(glm::vec3(width_ / 2.0f, height_ / 2.0f, 0.0f));
		config_.viewOffset = Vector2{ width_ / 2.0f, height_ / 2.0f };
		config_.viewBounds = Vector4{ 0, 0, width_, height_ };
		break;
	}
	
	glUniformMatrix4fv((GLuint)m_Matrices.viewportMatrixId, 1, false, glm::value_ptr(config_.viewport));
	glUniformMatrix4fv((GLuint)m_Matrices.projectionMatrixId, 1, false, glm::value_ptr(config_.projection));
}

void WindowSystem::OnWindowResized(WindowResized resized_)
{
	auto& [window_, width_, height_] = resized_;
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();
	config->windowWidth = width_;
	config->windowHeight = height_;

	SetViewProjectionMatrix(*config, *camera, (Float32)width_, (Float32)height_);
}

void WindowSystem::OnShaderChanged(ShaderChangeRequest request_)
{
	m_Matrices.cameraMatrixId = glGetUniformLocation(request_.m_Shader->programId, Shader::s_CameraMatrixName);
	m_Matrices.viewportMatrixId = glGetUniformLocation(request_.m_Shader->programId, Shader::s_ViewportMatrixName);
	m_Matrices.projectionMatrixId = glGetUniformLocation(request_.m_Shader->programId, Shader::s_ProjectionMatrixName);

	UpdateViewProjectionMatrix();
	UpdateCameraMatrix();
}

void Camera::Update()
{
	auto* window = Engine::GetDefaultResource<WindowSystem>();
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();

	bool changedProjection = false;
	
	window->UpdateViewProjectionMatrix(*config, *camera);
	window->UpdateCameraMatrix();

	Engine::Dispatcher().trigger<CameraUpdate>();
}

void WindowSystem::UpdateCameraMatrix()
{
	auto* camera = Engine::GetDefaultResource<Camera>();

	glm::mat4 scaleMatrix = glm::scale(glm::vec3(camera->zoom));
	m_Config.camera = scaleMatrix * glm::lookAt(camera->position, camera->position - glm::vec3(0, 0, 100), glm::vec3(0, 1, 0));
	glUniformMatrix4fv((GLuint)m_Matrices.cameraMatrixId, 1, false, glm::value_ptr(m_Config.camera));
}

void WindowSystem::SpinUp()
{
	Logger::info("Booting up renderer");

	m_Config.windowWidth = atoi(Engine::GetIniFile().GetValue("window", "width", "800"));
	m_Config.windowHeight = atoi(Engine::GetIniFile().GetValue("window", "height", "600"));
	m_Config.fullscreen = strcmp(Engine::GetIniFile().GetValue("window", "fullscreen", "false"), "true") == 0;
	m_Config.resizable = strcmp(Engine::GetIniFile().GetValue("window", "resizable", "false"), "true") == 0;

	assert(m_Config.windowWidth > 0 && m_Config.windowHeight > 0);

	auto& events = Engine::Dispatcher();

	if (!glfwInit())
	{
		events.trigger<Error>(Error{ "GLFW failed to initialize." });
		return;
	}

	glfwSetErrorCallback(ErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, m_Config.resizable ? GLFW_TRUE : GLFW_FALSE);

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

	const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
	const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
	Logger::critical("GPU Vendor: {}, renderer: {}", vendor, renderer);

	Engine::PutDefaultResource<RenderConfig>(&m_Config);
	Engine::PutDefaultResource<Camera>(&m_Camera);

	Engine::Dispatcher().sink<ShaderChangeRequest>().connect<&WindowSystem::OnShaderChanged>(this);
	Engine::Dispatcher().sink<WindowResized>().connect<&WindowSystem::OnWindowResized>(this);
	
	WindowResizeCallback(window, m_Config.windowWidth, m_Config.windowHeight);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCharCallback(window, CharCallback);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorCallback);
	glfwSetWindowSizeCallback(window, WindowResizeCallback);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

//	glEnable(GL_ALPHA_TEST);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
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
}

Vector2 Camera::WindowToScreen(Vector2 windowCoord_)
{
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();

	Vector2 cursorInScreen{ 0,0 };

	const auto mat = config->projection * config->camera;
	auto prod = (mat * glm::vec4(windowCoord_, 0, 0)) / (2 * camera->zoom);
	cursorInScreen.x = prod.x;
	cursorInScreen.y = prod.y;

	return cursorInScreen;
}

Vector2 Camera::WindowToWorld(Vector2 windowCoord_)
{
	Vector2 cursorInWorld{ 0,0 };
	auto* config = Engine::GetDefaultResource<RenderConfig>();

	auto pos = glm::unProject(Vector3{ windowCoord_ - config->viewOffset, 0 },
		config->camera, config->projection, config->viewBounds);
	cursorInWorld.x = pos.x;
	cursorInWorld.y = pos.y;
	return cursorInWorld;
}

Vector2 Camera::WorldToWindow(Vector2 worldCoord_)
{
	Vector2 cursorInWindow{ 0,0 };
	auto* config = Engine::GetDefaultResource<RenderConfig>();
	auto* camera = Engine::GetDefaultResource<Camera>();

	auto pos = glm::project(Vector3{ worldCoord_ + (camera->size / (2.0f * camera->zoom)), 0 },
		config->camera, config->projection, config->viewBounds);

	cursorInWindow.x = pos.x;
	cursorInWindow.y = pos.y;
	return cursorInWindow;
}
