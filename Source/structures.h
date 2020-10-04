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
