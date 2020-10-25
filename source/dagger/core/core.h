#pragma once

#include "core/view_ptr.h"

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>
#include <nlohmann/json.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>
#include <vector>
#include <fstream>
#include <chrono>

template<typename T>
using OwningPtr = std::unique_ptr<T>;

template<typename T>
using ViewPtr = jss::object_ptr<T>;

template<typename T>
using Sequence = std::vector<T>;

template<typename K, typename V>
using Map = tsl::sparse_map<K, V>;

template<typename K>
using Set = tsl::sparse_set<K>;

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

namespace Logger = spdlog;
namespace Files = std::filesystem;

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using Duration = std::chrono::duration<double>;

using FilePath = std::filesystem::path;
using FileInputStream = std::fstream;

using Matrix3 = glm::mat3x3;
using Matrix4 = glm::mat4x4;

using Vector2 = glm::fvec2;
using Vector3 = glm::fvec3;

using Color = glm::fvec4;

namespace JSON = nlohmann;

#define EMPTY_EVENT 

struct Log
{
	String message;
};

template<typename PhantomT>
struct AssetLoadRequest
{
    String path;
};

struct KeyboardEvent
{
	UInt32 key;
	UInt32 scancode;
	UInt32 action;
	UInt32 modifiers;
};

struct CharEvent
{
	UInt8 codepoint;
};

typedef glm::dvec2 ScrollEvent;

struct MouseEvent
{
	UInt32 button;
	UInt32 action;
	UInt32 modifiers;
};

typedef glm::dvec2 CursorEvent;

struct WindowResizedEvent
{
	UInt32 width;
	UInt32 height;
};

struct Error
{
	String message;
};

struct NextFrame EMPTY_EVENT {};
struct Exit EMPTY_EVENT {};
