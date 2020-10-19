#pragma once

#include <glm.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

typedef bool			Bool;
typedef std::uint8_t	UInt8;
typedef std::uint16_t	UInt16;
typedef std::uint32_t	UInt32;
typedef std::uint64_t	UInt64;

typedef std::int8_t		SInt8;
typedef std::int16_t	SInt16;
typedef std::int32_t	SInt32;
typedef std::int64_t	SInt64;

typedef float			Float32;
typedef double			Float64;

typedef unsigned char	Char;
typedef std::string		String;
typedef std::filesystem::path FilePath;

template<typename T>
using Vector = std::vector<T>;

typedef glm::fvec2		Position2D;
typedef glm::fvec3		Position3D;
typedef glm::fvec2		Size2D;
typedef glm::fvec3		Size3D;
typedef glm::fvec4		Color;

#define EMPTY_EVENT 

class Log
{
	String m_Message;

public:
	Log(String message_)
		: m_Message{ message_ }
	{}

	const String& Message() const
	{
		return m_Message;
	}
};

template<typename PhantomT>
struct AssetLoadRequest
{
    String m_Path;

	explicit AssetLoadRequest(String path_)
		: m_Path{ path_ }
	{}
};

struct KeyboardEvent
{
	UInt32 m_Key;
	UInt32 m_Scancode;
	UInt32 m_Action;
	UInt32 m_Modifiers;
};

struct CharEvent
{
	UInt8 m_Codepoint;
};

typedef glm::dvec2 ScrollEvent;

struct MouseEvent
{
	UInt32 m_Button;
	UInt32 m_Action;
	UInt32 m_Modifiers;
};

typedef glm::dvec2 CursorEvent;

struct Error
{
	String m_Message;

	Error(String message_)
		: m_Message{ message_ }
	{}

	Error() = delete;

	Error(const Error&) = default;
	Error& operator=(const Error&) = default;
	Error(Error&&) = default;
	Error& operator=(Error&&) = default;

	~Error() = default;
};

struct Frame EMPTY_EVENT {};
struct Exit EMPTY_EVENT {};
