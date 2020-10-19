#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>

template<typename T>
using Vector = std::vector<T>;

using Bool = bool;
using Char = char;
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using SInt8 = std::int8_t;
using SInt16 = std::int16_t;
using SInt32 = std::int32_t;
using SInt64 = std::int64_t;

using Float32 = float;
using Float64 = double;

using String = std::string;

namespace Files = std::filesystem;
using FilePath = std::filesystem::path;

using Matrix3 = glm::mat3x3;
using Matrix4 = glm::mat4x4;

using Vector2 = glm::fvec2;
using Vector3 = glm::fvec3;

using Color = glm::fvec4;

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

struct WindowResizedEvent
{
	UInt32 m_Width;
	UInt32 m_Height;
};

struct Error
{
	String m_Message;

	Error(String message_)
		: m_Message{ message_ }
	{}

	Error() = delete;
};

struct Frame EMPTY_EVENT {};
struct Exit EMPTY_EVENT {};
