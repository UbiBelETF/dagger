#pragma once

#include <cstdint>
#include <string>

struct GLFWwindow;

struct PreRender
{};

struct PostRender
{};

struct GUIRender
{};

struct RenderConfig
{
	std::uint32_t m_WindowWidth;
	std::uint32_t m_WindowHeight;

	GLFWwindow* m_Window;
};

struct KeyboardEvent
{
	int m_Key;
	int m_Scancode;
	int m_Action;
	int m_Modifiers;
};

struct MouseEvent
{
	int m_Button;
	int m_Action;
	int m_Modifiers;
};

struct CursorEvent
{
	double m_X;
	double m_Y;
};

struct Frame
{};

struct Exit
{};

struct Error
{
	std::string m_Message;

	Error(std::string message_)
		: m_Message{ message_ }
	{}

	Error() = delete;

	Error(const Error&) = default;
	Error& operator=(const Error&) = default;
	Error(Error&&) = default;
	Error& operator=(Error&&) = default;

	~Error() = default;
};
