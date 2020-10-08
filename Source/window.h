#pragma once

#include "system.h"
#include "structures.h"
#include <spdlog/spdlog.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include <deque>
#include <sstream>
 
using namespace dagger;

struct PreRender TRIGGER;
struct Render TRIGGER;
struct ToolRender TRIGGER;
struct PostRender TRIGGER;
struct GUIRender TRIGGER;

struct GLFWwindow;

struct RenderConfig
{
	GLsizei m_WindowWidth;
	GLsizei m_WindowHeight;

	GLFWwindow* m_Window;
};

struct WindowSystem 
	: public System
	, public Publisher<PreRender, Render, ToolRender, 
				KeyboardEvent, MouseEvent, CursorEvent, Error>
{
	RenderConfig m_Config;

	WindowSystem(GLsizei width_ = 640, GLsizei height_ = 480)
		: m_Config{ width_, height_ }
	{}

	~WindowSystem() = default;
	WindowSystem(const WindowSystem&) = delete;

	void SpinUp(Engine& engine_) override;
	void Run(Engine& engine_) override;
	void WindDown(Engine& engine_) override;
};