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

struct RenderConfig
{
	Bool fullscreen;
	Bool resizable;
	GLsizei windowWidth;
	GLsizei windowHeight;
	GLFWwindow* window;
	Matrix4 projection;
	Matrix4 viewport;
	Vector2 viewOffset;
	Vector2 lastSize;
	Vector4 viewBounds;
	Matrix4 camera;
};

struct CachedMatrices
{
	GLuint viewportMatrixId;
	GLuint projectionMatrixId;
	GLuint cameraMatrixId;
};

enum class ECameraMode
{
	ShowAsMuchAsPossible,
	FixedResolution
};

struct Camera
{
	ECameraMode mode{ ECameraMode::FixedResolution };
	Vector3 position{ 0, 0, 0 };
	Vector2 size{ 800, 600 };
	Float32 zoom{ 1 };

	void Update();

	static Vector2 WindowToScreen(Vector2 windowCoord_);
	static Vector2 WindowToWorld(Vector2 windowCoord_);
	static Vector2 WorldToWindow(Vector2 worldCoord_);
};

struct WindowSystem 
	: public System
	, public Publisher<PreRender, Render, ToolRender, 
				KeyboardEvent, MouseEvent, CursorEvent, Error>
{
	inline String SystemName() { return "Window System"; }

	CachedMatrices m_Matrices;
	RenderConfig m_Config;
	Camera m_Camera;

	WindowSystem()
		: m_Config{}
		, m_Camera{}
		, m_Matrices{}
	{}

	~WindowSystem() = default;
	WindowSystem(const WindowSystem&) = delete;

	void UpdateViewProjectionMatrix();

	void UpdateViewProjectionMatrix(RenderConfig& config_, Camera& camera_);
	void SetViewProjectionMatrix(RenderConfig& config_, Camera& camera_, Float32 width_, Float32 height_);

	void UpdateCameraMatrix();

	void OnWindowResized(WindowResized resized_);
	void OnShaderChanged(ShaderChangeRequest request_);
	
	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};
