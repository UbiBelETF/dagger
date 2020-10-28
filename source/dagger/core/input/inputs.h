#pragma once

#include "core/core.h"
#include "core/system.h"

#include <bitset>
#include <functional>

namespace dagger
{
    constexpr inline static UInt32 InputCount = 400;
    constexpr inline static UInt32 MouseStart = 150;

    struct InputAction
	{
        UInt32 trigger{ 0 };
        Float32 value{ 1 };
		UInt32 duration{ 0 };
		DaggerInputState event{ DaggerInputState::Pressed };
	};

	struct InputCommand
	{
		String name;
		Sequence<InputAction> actions;
	};

	struct InputContext
	{
        String name;
		Sequence<InputCommand> commands;
		BitSet<InputCount> bitmap;
	};

    struct InputReceiver
    {
        Sequence<String> contexts;
        Map<String, Float32> values;
    };

	struct InputState
	{
		Bool keys[InputCount]{ false, };
		Bool mouse[10]{ false, };
        Set<UInt32> releasedLastFrame{};
		Map<UInt32, TimePoint> moments{};
		BitSet<InputCount> bitmap;

		friend class InputSystem;
	};

	class InputSystem
		: public System
	{
        inline static Map<String, UInt32> s_InputValues =
        {
            { "KeyUnknown", (UInt32)DaggerKeyboard::KeyUnknown },
            { "KeySpace", (UInt32)DaggerKeyboard::KeySpace },
            { "KeyApostrophe", (UInt32)DaggerKeyboard::KeyApostrophe },
            { "KeyComma", (UInt32)DaggerKeyboard::KeyComma },
            { "KeyMinus", (UInt32)DaggerKeyboard::KeyMinus },
            { "KeyPeriod", (UInt32)DaggerKeyboard::KeyPeriod },
            { "KeySlash", (UInt32)DaggerKeyboard::KeySlash },
            { "Key0", (UInt32)DaggerKeyboard::Key0 },
            { "Key1", (UInt32)DaggerKeyboard::Key1 },
            { "Key2", (UInt32)DaggerKeyboard::Key2 },
            { "Key3", (UInt32)DaggerKeyboard::Key3 },
            { "Key4", (UInt32)DaggerKeyboard::Key4 },
            { "Key5", (UInt32)DaggerKeyboard::Key5 },
            { "Key6", (UInt32)DaggerKeyboard::Key6 },
            { "Key7", (UInt32)DaggerKeyboard::Key7 },
            { "Key8", (UInt32)DaggerKeyboard::Key8 },
            { "Key9", (UInt32)DaggerKeyboard::Key9 },
            { "KeySemicolon", (UInt32)DaggerKeyboard::KeySemicolon },
            { "KeyEqual", (UInt32)DaggerKeyboard::KeyEqual },
            { "KeyA", (UInt32)DaggerKeyboard::KeyA },
            { "KeyB", (UInt32)DaggerKeyboard::KeyB },
            { "KeyC", (UInt32)DaggerKeyboard::KeyC },
            { "KeyD", (UInt32)DaggerKeyboard::KeyD },
            { "KeyE", (UInt32)DaggerKeyboard::KeyE },
            { "KeyF", (UInt32)DaggerKeyboard::KeyF },
            { "KeyG", (UInt32)DaggerKeyboard::KeyG },
            { "KeyH", (UInt32)DaggerKeyboard::KeyH },
            { "KeyI", (UInt32)DaggerKeyboard::KeyI },
            { "KeyJ", (UInt32)DaggerKeyboard::KeyJ },
            { "KeyK", (UInt32)DaggerKeyboard::KeyK },
            { "KeyL", (UInt32)DaggerKeyboard::KeyL },
            { "KeyM", (UInt32)DaggerKeyboard::KeyM },
            { "KeyN", (UInt32)DaggerKeyboard::KeyN },
            { "KeyO", (UInt32)DaggerKeyboard::KeyO },
            { "KeyP", (UInt32)DaggerKeyboard::KeyP },
            { "KeyQ", (UInt32)DaggerKeyboard::KeyQ },
            { "KeyR", (UInt32)DaggerKeyboard::KeyR },
            { "KeyS", (UInt32)DaggerKeyboard::KeyS },
            { "KeyT", (UInt32)DaggerKeyboard::KeyT },
            { "KeyU", (UInt32)DaggerKeyboard::KeyU },
            { "KeyV", (UInt32)DaggerKeyboard::KeyV },
            { "KeyW", (UInt32)DaggerKeyboard::KeyW },
            { "KeyX", (UInt32)DaggerKeyboard::KeyX },
            { "KeyY", (UInt32)DaggerKeyboard::KeyY },
            { "KeyZ", (UInt32)DaggerKeyboard::KeyZ },
            { "KeyLeftBracket", (UInt32)DaggerKeyboard::KeyLeftBracket },
            { "KeyBackslash", (UInt32)DaggerKeyboard::KeyBackslash },
            { "KeyRightBracket", (UInt32)DaggerKeyboard::KeyRightBracket },
            { "KeyGraveAccent", (UInt32)DaggerKeyboard::KeyGraveAccent },
            { "KeyWorld1", (UInt32)DaggerKeyboard::KeyWorld1 },
            { "KeyWorld2", (UInt32)DaggerKeyboard::KeyWorld2 },
            { "KeyEscape", (UInt32)DaggerKeyboard::KeyEscape },
            { "KeyEnter", (UInt32)DaggerKeyboard::KeyEnter },
            { "KeyTab", (UInt32)DaggerKeyboard::KeyTab },
            { "KeyBackspace", (UInt32)DaggerKeyboard::KeyBackspace },
            { "KeyInsert", (UInt32)DaggerKeyboard::KeyInsert },
            { "KeyDelete", (UInt32)DaggerKeyboard::KeyDelete },
            { "KeyRight", (UInt32)DaggerKeyboard::KeyRight },
            { "KeyLeft", (UInt32)DaggerKeyboard::KeyLeft },
            { "KeyDown", (UInt32)DaggerKeyboard::KeyDown },
            { "KeyUp", (UInt32)DaggerKeyboard::KeyUp },
            { "KeyPageUp", (UInt32)DaggerKeyboard::KeyPageUp },
            { "KeyPageDown", (UInt32)DaggerKeyboard::KeyPageDown },
            { "KeyHome", (UInt32)DaggerKeyboard::KeyHome },
            { "KeyEnd", (UInt32)DaggerKeyboard::KeyEnd },
            { "KeyCapsLock", (UInt32)DaggerKeyboard::KeyCapsLock },
            { "KeyScrollLock", (UInt32)DaggerKeyboard::KeyScrollLock },
            { "KeyNumLock", (UInt32)DaggerKeyboard::KeyNumLock },
            { "KeyPrintScreen", (UInt32)DaggerKeyboard::KeyPrintScreen },
            { "KeyPause", (UInt32)DaggerKeyboard::KeyPause },
            { "KeyF1", (UInt32)DaggerKeyboard::KeyF1 },
            { "KeyF2", (UInt32)DaggerKeyboard::KeyF2 },
            { "KeyF3", (UInt32)DaggerKeyboard::KeyF3 },
            { "KeyF4", (UInt32)DaggerKeyboard::KeyF4 },
            { "KeyF5", (UInt32)DaggerKeyboard::KeyF5 },
            { "KeyF6", (UInt32)DaggerKeyboard::KeyF6 },
            { "KeyF7", (UInt32)DaggerKeyboard::KeyF7 },
            { "KeyF8", (UInt32)DaggerKeyboard::KeyF8 },
            { "KeyF9", (UInt32)DaggerKeyboard::KeyF9 },
            { "KeyF10", (UInt32)DaggerKeyboard::KeyF10 },
            { "KeyF11", (UInt32)DaggerKeyboard::KeyF11 },
            { "KeyF12", (UInt32)DaggerKeyboard::KeyF12 },
            { "KeyF13", (UInt32)DaggerKeyboard::KeyF13 },
            { "KeyF14", (UInt32)DaggerKeyboard::KeyF14 },
            { "KeyF15", (UInt32)DaggerKeyboard::KeyF15 },
            { "KeyF16", (UInt32)DaggerKeyboard::KeyF16 },
            { "KeyF17", (UInt32)DaggerKeyboard::KeyF17 },
            { "KeyF18", (UInt32)DaggerKeyboard::KeyF18 },
            { "KeyF19", (UInt32)DaggerKeyboard::KeyF19 },
            { "KeyF20", (UInt32)DaggerKeyboard::KeyF20 },
            { "KeyF21", (UInt32)DaggerKeyboard::KeyF21 },
            { "KeyF22", (UInt32)DaggerKeyboard::KeyF22 },
            { "KeyF23", (UInt32)DaggerKeyboard::KeyF23 },
            { "KeyF24", (UInt32)DaggerKeyboard::KeyF24 },
            { "KeyF25", (UInt32)DaggerKeyboard::KeyF25 },
            { "KeyKp0", (UInt32)DaggerKeyboard::KeyKp0 },
            { "KeyKp1", (UInt32)DaggerKeyboard::KeyKp1 },
            { "KeyKp2", (UInt32)DaggerKeyboard::KeyKp2 },
            { "KeyKp3", (UInt32)DaggerKeyboard::KeyKp3 },
            { "KeyKp4", (UInt32)DaggerKeyboard::KeyKp4 },
            { "KeyKp5", (UInt32)DaggerKeyboard::KeyKp5 },
            { "KeyKp6", (UInt32)DaggerKeyboard::KeyKp6 },
            { "KeyKp7", (UInt32)DaggerKeyboard::KeyKp7 },
            { "KeyKp8", (UInt32)DaggerKeyboard::KeyKp8 },
            { "KeyKp9", (UInt32)DaggerKeyboard::KeyKp9 },
            { "KeyKpDecimal", (UInt32)DaggerKeyboard::KeyKpDecimal },
            { "KeyKpDivide", (UInt32)DaggerKeyboard::KeyKpDivide },
            { "KeyKpMultiply", (UInt32)DaggerKeyboard::KeyKpMultiply },
            { "KeyKpSubtract", (UInt32)DaggerKeyboard::KeyKpSubtract },
            { "KeyKpAdd", (UInt32)DaggerKeyboard::KeyKpAdd },
            { "KeyKpEnter", (UInt32)DaggerKeyboard::KeyKpEnter },
            { "KeyKpEqual", (UInt32)DaggerKeyboard::KeyKpEqual },
            { "KeyLeftShift", (UInt32)DaggerKeyboard::KeyLeftShift },
            { "KeyLeftControl", (UInt32)DaggerKeyboard::KeyLeftControl },
            { "KeyLeftAlt", (UInt32)DaggerKeyboard::KeyLeftAlt },
            { "KeyLeftSuper", (UInt32)DaggerKeyboard::KeyLeftSuper },
            { "KeyRightShift", (UInt32)DaggerKeyboard::KeyRightShift },
            { "KeyRightControl", (UInt32)DaggerKeyboard::KeyRightControl },
            { "KeyRightAlt", (UInt32)DaggerKeyboard::KeyRightAlt },
            { "KeyRightSuper", (UInt32)DaggerKeyboard::KeyRightSuper },
            { "KeyMenu", (UInt32)DaggerKeyboard::KeyMenu },
            { "MouseButton1", (UInt32)DaggerMouse::MouseButton1 },
            { "MouseButton2", (UInt32)DaggerMouse::MouseButton2 },
            { "MouseButton3", (UInt32)DaggerMouse::MouseButton3 },
            { "MouseButton4", (UInt32)DaggerMouse::MouseButton4 },
            { "MouseButton5", (UInt32)DaggerMouse::MouseButton5 },
            { "MouseButton6", (UInt32)DaggerMouse::MouseButton6 },
            { "MouseButton7", (UInt32)DaggerMouse::MouseButton7 },
            { "MouseButton8", (UInt32)DaggerMouse::MouseButton8 }
        };

		void LoadDefaultAssets();
		void OnAssetLoadRequest(AssetLoadRequest<InputContext> request_);

		void LoadInputAction(InputCommand& command_, JSON::json& input_);

		void OnKeyboardEvent(KeyboardEvent input_);
		void OnMouseEvent(MouseEvent input_);

	public:
		InputState m_InputState;

		inline String SystemName() { return "Input System"; }

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};

	namespace input
	{
		static Bool IsInputDown(DaggerKeyboard key_);
		static Bool IsInputDown(DaggerMouse button_);

		static UInt32 GetInputDuration(DaggerKeyboard key_);
		static UInt32 GetInputDuration(DaggerMouse mouse_);

		static void ConsumeInput(DaggerKeyboard key_);
		static void ConsumeInput(DaggerMouse button_);
	}
}