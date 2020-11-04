#pragma once

#include "shaders.h"
#include "core/system.h"
#include "core/core.h"

#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <deque>
#include <sstream>

using namespace dagger;

struct PreRender {};
struct Render {};
struct ToolRender {};
struct PostRender {};
struct GUIRender {};

struct GLFWwindow;

struct WindowResized
{
	GLFWwindow* window;
	UInt32 width;
	UInt32 height;
};

// config provided from json file
struct GameRenderConfig
{
    bool lockScreenResize;
    bool fullscreen;
    GLsizei windowWidth;
    GLsizei windowHeight;
};

struct RenderConfig
{
    bool lockScreenResize;
    bool fullscreen;
    GLsizei windowWidth;
    GLsizei windowHeight;

    GLFWwindow* window;
    Matrix4 projection;
    Matrix4 cameraView;

    RenderConfig(GameRenderConfig config_)
        : lockScreenResize(config_.lockScreenResize),
        fullscreen(config_.fullscreen),
        windowWidth(config_.windowWidth),
        windowHeight(config_.windowHeight)
    {}
};

enum class ECameraMode
{
	ShowAsMuchAsPossible,
	FixedWidth,
	FixedHeight,
	FixedResolution
};

struct Camera
{
	ECameraMode mode{ ECameraMode::FixedWidth };
	Vector2 size{ 800, 600 };
	Vector2 position{ 0, 0 };
	Float32 zoom{ 1 };
};

struct WindowSystem 
	: public System
	, public Publisher<PreRender, Render, ToolRender, 
				KeyboardEvent, MouseEvent, CursorEvent, Error>
{
	inline String SystemName() { return "Window System"; }

	RenderConfig m_Config;
	Camera m_Camera;

	WindowSystem()
        : m_Config(GameRenderConfig{ false, true, 0, 0 })
		, m_Camera{}
	{}

	WindowSystem(GameRenderConfig config_)
		: m_Config{config_}
		, m_Camera{}
	{}

	~WindowSystem() = default;
	WindowSystem(const WindowSystem&) = delete;

	void OnWindowResized(WindowResized resized_);
	void OnShaderChanged(ShaderChangeRequest request_);
	void OnCameraUpdated(Camera camera_);

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};