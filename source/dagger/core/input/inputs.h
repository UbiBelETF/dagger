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
        EDaggerInputState event{ EDaggerInputState::Pressed };
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
        Vector2 cursor{ 0, 0 };
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
            { "KeyUnknown", (UInt32)EDaggerKeyboard::KeyUnknown },
            { "KeySpace", (UInt32)EDaggerKeyboard::KeySpace },
            { "KeyApostrophe", (UInt32)EDaggerKeyboard::KeyApostrophe },
            { "KeyComma", (UInt32)EDaggerKeyboard::KeyComma },
            { "KeyMinus", (UInt32)EDaggerKeyboard::KeyMinus },
            { "KeyPeriod", (UInt32)EDaggerKeyboard::KeyPeriod },
            { "KeySlash", (UInt32)EDaggerKeyboard::KeySlash },
            { "Key0", (UInt32)EDaggerKeyboard::Key0 },
            { "Key1", (UInt32)EDaggerKeyboard::Key1 },
            { "Key2", (UInt32)EDaggerKeyboard::Key2 },
            { "Key3", (UInt32)EDaggerKeyboard::Key3 },
            { "Key4", (UInt32)EDaggerKeyboard::Key4 },
            { "Key5", (UInt32)EDaggerKeyboard::Key5 },
            { "Key6", (UInt32)EDaggerKeyboard::Key6 },
            { "Key7", (UInt32)EDaggerKeyboard::Key7 },
            { "Key8", (UInt32)EDaggerKeyboard::Key8 },
            { "Key9", (UInt32)EDaggerKeyboard::Key9 },
            { "KeySemicolon", (UInt32)EDaggerKeyboard::KeySemicolon },
            { "KeyEqual", (UInt32)EDaggerKeyboard::KeyEqual },
            { "KeyA", (UInt32)EDaggerKeyboard::KeyA },
            { "KeyB", (UInt32)EDaggerKeyboard::KeyB },
            { "KeyC", (UInt32)EDaggerKeyboard::KeyC },
            { "KeyD", (UInt32)EDaggerKeyboard::KeyD },
            { "KeyE", (UInt32)EDaggerKeyboard::KeyE },
            { "KeyF", (UInt32)EDaggerKeyboard::KeyF },
            { "KeyG", (UInt32)EDaggerKeyboard::KeyG },
            { "KeyH", (UInt32)EDaggerKeyboard::KeyH },
            { "KeyI", (UInt32)EDaggerKeyboard::KeyI },
            { "KeyJ", (UInt32)EDaggerKeyboard::KeyJ },
            { "KeyK", (UInt32)EDaggerKeyboard::KeyK },
            { "KeyL", (UInt32)EDaggerKeyboard::KeyL },
            { "KeyM", (UInt32)EDaggerKeyboard::KeyM },
            { "KeyN", (UInt32)EDaggerKeyboard::KeyN },
            { "KeyO", (UInt32)EDaggerKeyboard::KeyO },
            { "KeyP", (UInt32)EDaggerKeyboard::KeyP },
            { "KeyQ", (UInt32)EDaggerKeyboard::KeyQ },
            { "KeyR", (UInt32)EDaggerKeyboard::KeyR },
            { "KeyS", (UInt32)EDaggerKeyboard::KeyS },
            { "KeyT", (UInt32)EDaggerKeyboard::KeyT },
            { "KeyU", (UInt32)EDaggerKeyboard::KeyU },
            { "KeyV", (UInt32)EDaggerKeyboard::KeyV },
            { "KeyW", (UInt32)EDaggerKeyboard::KeyW },
            { "KeyX", (UInt32)EDaggerKeyboard::KeyX },
            { "KeyY", (UInt32)EDaggerKeyboard::KeyY },
            { "KeyZ", (UInt32)EDaggerKeyboard::KeyZ },
            { "KeyLeftBracket", (UInt32)EDaggerKeyboard::KeyLeftBracket },
            { "KeyBackslash", (UInt32)EDaggerKeyboard::KeyBackslash },
            { "KeyRightBracket", (UInt32)EDaggerKeyboard::KeyRightBracket },
            { "KeyGraveAccent", (UInt32)EDaggerKeyboard::KeyGraveAccent },
            { "KeyWorld1", (UInt32)EDaggerKeyboard::KeyWorld1 },
            { "KeyWorld2", (UInt32)EDaggerKeyboard::KeyWorld2 },
            { "KeyEscape", (UInt32)EDaggerKeyboard::KeyEscape },
            { "KeyEnter", (UInt32)EDaggerKeyboard::KeyEnter },
            { "KeyTab", (UInt32)EDaggerKeyboard::KeyTab },
            { "KeyBackspace", (UInt32)EDaggerKeyboard::KeyBackspace },
            { "KeyInsert", (UInt32)EDaggerKeyboard::KeyInsert },
            { "KeyDelete", (UInt32)EDaggerKeyboard::KeyDelete },
            { "KeyRight", (UInt32)EDaggerKeyboard::KeyRight },
            { "KeyLeft", (UInt32)EDaggerKeyboard::KeyLeft },
            { "KeyDown", (UInt32)EDaggerKeyboard::KeyDown },
            { "KeyUp", (UInt32)EDaggerKeyboard::KeyUp },
            { "KeyPageUp", (UInt32)EDaggerKeyboard::KeyPageUp },
            { "KeyPageDown", (UInt32)EDaggerKeyboard::KeyPageDown },
            { "KeyHome", (UInt32)EDaggerKeyboard::KeyHome },
            { "KeyEnd", (UInt32)EDaggerKeyboard::KeyEnd },
            { "KeyCapsLock", (UInt32)EDaggerKeyboard::KeyCapsLock },
            { "KeyScrollLock", (UInt32)EDaggerKeyboard::KeyScrollLock },
            { "KeyNumLock", (UInt32)EDaggerKeyboard::KeyNumLock },
            { "KeyPrintScreen", (UInt32)EDaggerKeyboard::KeyPrintScreen },
            { "KeyPause", (UInt32)EDaggerKeyboard::KeyPause },
            { "KeyF1", (UInt32)EDaggerKeyboard::KeyF1 },
            { "KeyF2", (UInt32)EDaggerKeyboard::KeyF2 },
            { "KeyF3", (UInt32)EDaggerKeyboard::KeyF3 },
            { "KeyF4", (UInt32)EDaggerKeyboard::KeyF4 },
            { "KeyF5", (UInt32)EDaggerKeyboard::KeyF5 },
            { "KeyF6", (UInt32)EDaggerKeyboard::KeyF6 },
            { "KeyF7", (UInt32)EDaggerKeyboard::KeyF7 },
            { "KeyF8", (UInt32)EDaggerKeyboard::KeyF8 },
            { "KeyF9", (UInt32)EDaggerKeyboard::KeyF9 },
            { "KeyF10", (UInt32)EDaggerKeyboard::KeyF10 },
            { "KeyF11", (UInt32)EDaggerKeyboard::KeyF11 },
            { "KeyF12", (UInt32)EDaggerKeyboard::KeyF12 },
            { "KeyF13", (UInt32)EDaggerKeyboard::KeyF13 },
            { "KeyF14", (UInt32)EDaggerKeyboard::KeyF14 },
            { "KeyF15", (UInt32)EDaggerKeyboard::KeyF15 },
            { "KeyF16", (UInt32)EDaggerKeyboard::KeyF16 },
            { "KeyF17", (UInt32)EDaggerKeyboard::KeyF17 },
            { "KeyF18", (UInt32)EDaggerKeyboard::KeyF18 },
            { "KeyF19", (UInt32)EDaggerKeyboard::KeyF19 },
            { "KeyF20", (UInt32)EDaggerKeyboard::KeyF20 },
            { "KeyF21", (UInt32)EDaggerKeyboard::KeyF21 },
            { "KeyF22", (UInt32)EDaggerKeyboard::KeyF22 },
            { "KeyF23", (UInt32)EDaggerKeyboard::KeyF23 },
            { "KeyF24", (UInt32)EDaggerKeyboard::KeyF24 },
            { "KeyF25", (UInt32)EDaggerKeyboard::KeyF25 },
            { "KeyKp0", (UInt32)EDaggerKeyboard::KeyKp0 },
            { "KeyKp1", (UInt32)EDaggerKeyboard::KeyKp1 },
            { "KeyKp2", (UInt32)EDaggerKeyboard::KeyKp2 },
            { "KeyKp3", (UInt32)EDaggerKeyboard::KeyKp3 },
            { "KeyKp4", (UInt32)EDaggerKeyboard::KeyKp4 },
            { "KeyKp5", (UInt32)EDaggerKeyboard::KeyKp5 },
            { "KeyKp6", (UInt32)EDaggerKeyboard::KeyKp6 },
            { "KeyKp7", (UInt32)EDaggerKeyboard::KeyKp7 },
            { "KeyKp8", (UInt32)EDaggerKeyboard::KeyKp8 },
            { "KeyKp9", (UInt32)EDaggerKeyboard::KeyKp9 },
            { "KeyKpDecimal", (UInt32)EDaggerKeyboard::KeyKpDecimal },
            { "KeyKpDivide", (UInt32)EDaggerKeyboard::KeyKpDivide },
            { "KeyKpMultiply", (UInt32)EDaggerKeyboard::KeyKpMultiply },
            { "KeyKpSubtract", (UInt32)EDaggerKeyboard::KeyKpSubtract },
            { "KeyKpAdd", (UInt32)EDaggerKeyboard::KeyKpAdd },
            { "KeyKpEnter", (UInt32)EDaggerKeyboard::KeyKpEnter },
            { "KeyKpEqual", (UInt32)EDaggerKeyboard::KeyKpEqual },
            { "KeyLeftShift", (UInt32)EDaggerKeyboard::KeyLeftShift },
            { "KeyLeftControl", (UInt32)EDaggerKeyboard::KeyLeftControl },
            { "KeyLeftAlt", (UInt32)EDaggerKeyboard::KeyLeftAlt },
            { "KeyLeftSuper", (UInt32)EDaggerKeyboard::KeyLeftSuper },
            { "KeyRightShift", (UInt32)EDaggerKeyboard::KeyRightShift },
            { "KeyRightControl", (UInt32)EDaggerKeyboard::KeyRightControl },
            { "KeyRightAlt", (UInt32)EDaggerKeyboard::KeyRightAlt },
            { "KeyRightSuper", (UInt32)EDaggerKeyboard::KeyRightSuper },
            { "KeyMenu", (UInt32)EDaggerKeyboard::KeyMenu },
            { "MouseButton1", (UInt32)EDaggerMouse::MouseButton1 },
            { "MouseButton2", (UInt32)EDaggerMouse::MouseButton2 },
            { "MouseButton3", (UInt32)EDaggerMouse::MouseButton3 },
            { "MouseButton4", (UInt32)EDaggerMouse::MouseButton4 },
            { "MouseButton5", (UInt32)EDaggerMouse::MouseButton5 },
            { "MouseButton6", (UInt32)EDaggerMouse::MouseButton6 },
            { "MouseButton7", (UInt32)EDaggerMouse::MouseButton7 },
            { "MouseButton8", (UInt32)EDaggerMouse::MouseButton8 }
        };

        void LoadDefaultAssets();
        void OnAssetLoadRequest(AssetLoadRequest<InputContext> request_);

        void LoadInputAction(InputCommand& command_, JSON::json& input_);

        void OnKeyboardEvent(KeyboardEvent input_);
        void OnMouseEvent(MouseEvent input_);
        void OnCursorMoveEvent(CursorEvent cursor_);

    public:
        InputState m_InputState;

        inline String SystemName() { return "Input System"; }

        void SpinUp() override;
        void Run() override;
        void WindDown() override;
    };

    struct Input
    {
        static Bool IsInputDown(EDaggerKeyboard key_);
        static Bool IsInputDown(EDaggerMouse button_);

        static const Vector2 CursorPositionInWindow();
        static const Vector2 CursorPositionInScreen();
        static const Vector2 CursorPositionInWorld();

        static UInt32 GetInputDuration(EDaggerKeyboard key_);
        static UInt32 GetInputDuration(EDaggerMouse mouse_);

        static void ConsumeInput(EDaggerKeyboard key_);
        static void ConsumeInput(EDaggerMouse button_);
    };
}