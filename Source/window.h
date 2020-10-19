#pragma once

#include "system.h"
#include "structures.h"
#include <spdlog/spdlog.h>

#include "glad.h"
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

struct RenderConfig
{
	bool m_Fullscreen;
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

	WindowSystem()
		: m_Config{ true, 0, 0 }
	{}

	WindowSystem(GLsizei width_, GLsizei height_)
		: m_Config{ false, width_, height_ }
	{}

	~WindowSystem() = default;
	WindowSystem(const WindowSystem&) = delete;

	void SpinUp() override;
	void Run() override;
	void WindDown() override;
};