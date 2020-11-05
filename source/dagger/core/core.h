#pragma once

#define MEASURE_SYSTEMS

#include "core/view_ptr.h"
#include "core/filesystem.h"
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <vector>
#include <fstream>
#include <chrono>
#include <bitset>

#include <SimpleIni.h>

using IniFile = CSimpleIni;

using Registry = entt::registry;
using Entity = entt::entity;

// OwningPtr<T>: the pointer is owned and destroyed by whoever holds this instance.
template<typename T>
using OwningPtr = std::unique_ptr<T>;

// ViewPtr<T>: the pointer is not owned or managed by whoever holds this instance.
// It just views the object through this. Can be used anywhere a raw pointer could be used.
template<typename T>
using ViewPtr = jss::object_ptr<T>;

// Pair<A, B>: contains two types with no other relations in it.
template<typename A, typename B>
using Pair = std::pair<A, B>;

#define pair(X, Y)  std::make_pair((X), (Y))

// StaticArray<T, N>: a contiguous sequence of statically-sized elements in memory.
template<typename T, int N>
using StaticArray = std::array<T, N>;

// Sequence<T>: a contiguous sequence of elements in memory.
template<typename T>
using Sequence = std::vector<T>;

// Map<K, V>: a quick map with keys of type K and elements of type V.
template<typename K, typename V>
using Map = tsl::sparse_map<K, V>;

// Set<E>: a quick set of elements of type E
template<typename K>
using Set = tsl::sparse_set<K>;

// BitSet<N>: a contiguous bitset with N elements. Can be used for quick membership checks.
// Basically the same as a compile-time Sequence<Bool> with fixed size N.
template<int N>
using BitSet = std::bitset<N>;

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

// Logger: provides nice logging functions like info, error, debug, critical, etc.
namespace Logger = spdlog;

// Files: provides nice filesystem functions and abstractions.
namespace Files = std::filesystem;
using FilePath = std::filesystem::path;
using FileInputStream = std::fstream;

// TimePoint: a literal point in time. It has no unit by itself.
using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

// Duration: a difference between two TimePoints.
using Duration = std::chrono::duration<Float32>;

// DurationToMilliseconds: turns a unit-less Duration into the count of milliseconds.
#define DurationToMilliseconds(X)   (std::chrono::duration_cast<std::chrono::milliseconds>(X).count())

// TimeSnapshot: gets you the current TimePoint.
#define TimeSnapshot    std::chrono::steady_clock::now

using Matrix3 = glm::mat3x3;
using Matrix4 = glm::mat4x4;

using Vector2 = glm::fvec2;
using Vector3 = glm::fvec3;

using Color = glm::fvec4;

namespace JSON = nlohmann;

#define EMPTY_EVENT 

enum class EDaggerKeyboard
{
    KeyUnknown = 1,
    KeySpace = 32,
    KeyApostrophe = 39,
    KeyComma = 44,
    KeyMinus = 45,
    KeyPeriod = 46,
    KeySlash = 47,
    Key0 = 48,
    Key1 = 49,
    Key2 = 50,
    Key3 = 51,
    Key4 = 52,
    Key5 = 53,
    Key6 = 54,
    Key7 = 55,
    Key8 = 56,
    Key9 = 57,
    KeySemicolon = 59,
    KeyEqual = 61,
    KeyA = 65,
    KeyB = 66,
    KeyC = 67,
    KeyD = 68,
    KeyE = 69,
    KeyF = 70,
    KeyG = 71,
    KeyH = 72,
    KeyI = 73,
    KeyJ = 74,
    KeyK = 75,
    KeyL = 76,
    KeyM = 77,
    KeyN = 78,
    KeyO = 79,
    KeyP = 80,
    KeyQ = 81,
    KeyR = 82,
    KeyS = 83,
    KeyT = 84,
    KeyU = 85,
    KeyV = 86,
    KeyW = 87,
    KeyX = 88,
    KeyY = 89,
    KeyZ = 90,
    KeyLeftBracket = 91,
    KeyBackslash = 92,
    KeyRightBracket = 93,
    KeyGraveAccent = 96,
    KeyWorld1 = 161,
    KeyWorld2 = 162,
    KeyEscape = 256,
    KeyEnter = 257,
    KeyTab = 258,
    KeyBackspace = 259,
    KeyInsert = 260,
    KeyDelete = 261,
    KeyRight = 262,
    KeyLeft = 263,
    KeyDown = 264,
    KeyUp = 265,
    KeyPageUp = 266,
    KeyPageDown = 267,
    KeyHome = 268,
    KeyEnd = 269,
    KeyCapsLock = 280,
    KeyScrollLock = 281,
    KeyNumLock = 282,
    KeyPrintScreen = 283,
    KeyPause = 284,
    KeyF1 = 290,
    KeyF2 = 291,
    KeyF3 = 292,
    KeyF4 = 293,
    KeyF5 = 294,
    KeyF6 = 295,
    KeyF7 = 296,
    KeyF8 = 297,
    KeyF9 = 298,
    KeyF10 = 299,
    KeyF11 = 300,
    KeyF12 = 301,
    KeyF13 = 302,
    KeyF14 = 303,
    KeyF15 = 304,
    KeyF16 = 305,
    KeyF17 = 306,
    KeyF18 = 307,
    KeyF19 = 308,
    KeyF20 = 309,
    KeyF21 = 310,
    KeyF22 = 311,
    KeyF23 = 312,
    KeyF24 = 313,
    KeyF25 = 314,
    KeyKp0 = 320,
    KeyKp1 = 321,
    KeyKp2 = 322,
    KeyKp3 = 323,
    KeyKp4 = 324,
    KeyKp5 = 325,
    KeyKp6 = 326,
    KeyKp7 = 327,
    KeyKp8 = 328,
    KeyKp9 = 329,
    KeyKpDecimal = 330,
    KeyKpDivide = 331,
    KeyKpMultiply = 332,
    KeyKpSubtract = 333,
    KeyKpAdd = 334,
    KeyKpEnter = 335,
    KeyKpEqual = 336,
    KeyLeftShift = 340,
    KeyLeftControl = 341,
    KeyLeftAlt = 342,
    KeyLeftSuper = 343,
    KeyRightShift = 344,
    KeyRightControl = 345,
    KeyRightAlt = 346,
    KeyRightSuper = 347,
    KeyMenu = 348,
};

enum class EDaggerMouse
{
    MouseButton1 = 150,
    MouseButton2 = 151,
    MouseButton3 = 152,
    MouseButton4 = 153,
    MouseButton5 = 154,
    MouseButton6 = 155,
    MouseButton7 = 156,
    MouseButton8 = 157,
};

enum class EDaggerInputState
{
    Released = 0,
    Pressed = 1,
    Held = 2,
};

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
	EDaggerKeyboard key;
    EDaggerInputState action;
    UInt32 scancode;
	UInt32 modifiers;
};

struct CharEvent
{
	UInt8 codepoint;
};

typedef glm::dvec2 ScrollEvent;

struct MouseEvent
{
	EDaggerMouse button;
	EDaggerInputState action;
	UInt32 modifiers;
};

typedef glm::dvec2 CursorEvent;

struct Error
{
	String message;
};

struct SystemRunStats
{
	String name;
	Duration length;
};

struct NextFrame EMPTY_EVENT {};
struct Exit EMPTY_EVENT {};
