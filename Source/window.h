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
	std::uint32_t m_WindowWidth;
	std::uint32_t m_WindowHeight;

	GLFWwindow* m_Window;
};

struct WindowSystem 
	: public System
	, public Publisher<PreRender, Render, ToolRender, 
				KeyboardEvent, MouseEvent, CursorEvent, Error>
{
	RenderConfig m_Config;

	WindowSystem(std::uint32_t width_ = 640, std::uint32_t height_ = 480)
		: m_Config{ width_, height_ }
	{}

	~WindowSystem() = default;
	WindowSystem(const WindowSystem&) = delete;

	void SpinUp(Engine& engine_) override;
	void Run(Engine& engine_) override;
	void WindDown(Engine& engine_) override;
};