#pragma once

#include <cstdint>
#include <string>

class Command
{
	std::string m_Query;

public:
	Command(std::string query_)
		: m_Query{ query_ }
	{}

	const std::string& Query() const
	{
		return m_Query;
	}
};

class Log
{
	std::string m_Message;

public:
	Log(std::string message_)
		: m_Message{ message_ }
	{}

	const std::string& Message() const
	{
		return m_Message;
	}

};

#define TRIGGER {}

template<typename PhantomT>
struct AssetLoadRequest
{
    std::string m_Path;

	explicit AssetLoadRequest(std::string path_)
		: m_Path{ path_ }
	{}
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

struct Frame TRIGGER;
struct Exit TRIGGER;

struct Position2D
{
    float X, Y;
};

struct Position3D
{
    float X, Y, Z;
};

struct Color
{
    float R, G, B, A;
};

struct Sprite
{
    Position3D m_Position{ 0, 0, 0 };		// 3
    Position2D m_Pivot{ 0, 0 };				// 2
    Color m_Color{ 0.5f, 0.5f,0.5f, 1.0f }; // 4
	unsigned int m_Image{ 0 };				// 1
};
