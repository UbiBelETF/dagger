#include "inputs.h"
#include "core/engine.h"

#include <GLFW/glfw3.h>

using namespace dagger;

void InputSystem::OnKeyboardEvent(KeyboardEvent input_)
{
	auto key = (UInt64)input_.key;

	if (input_.action == DaggerInputState::Pressed)
	{
		m_CurrentInputPattern.keys[key] = true;
		m_CurrentInputPattern.moment[key] = Engine::CurrentTime();
		m_Bitmap.set(key);
	}
	else if (input_.action == DaggerInputState::Released)
	{
		m_CurrentInputPattern.keys[key] = false;
		m_CurrentInputPattern.moment.erase(key);
		m_Bitmap.reset(key);
	}
}

void InputSystem::OnMouseEvent(MouseEvent input_)
{
	UInt32 button = (UInt64)input_.button;

	if (input_.action == DaggerInputState::Pressed)
	{
		m_CurrentInputPattern.mouse[button] = true;
		m_CurrentInputPattern.moment[button] = std::chrono::steady_clock::now();
		m_Bitmap.set(button);
	}
	else if (input_.action == DaggerInputState::Released)
	{
		m_CurrentInputPattern.mouse[button] = false;
		m_CurrentInputPattern.moment.erase(button);
		m_Bitmap.reset(button);
	}
}

void InputSystem::Test(InputPattern pattern_)
{
	if (pattern_.IsInputDown(DaggerKey::KEY_A))
	{
		if (pattern_.GetInputLength(DaggerKey::KEY_A) > 1000)
			Logger::critical("A held for a second!");
	}
}

void InputSystem::SpinUp() 
{
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().connect<&InputSystem::OnMouseEvent>(this);
	Engine::Dispatcher().sink<InputPattern>().connect<&InputSystem::Test>(this);
};

void InputSystem::Run()
{
	if (m_Bitmap.count() > 0)
	{
		Engine::Dispatcher().trigger<InputPattern>(m_CurrentInputPattern);
	}
};

void InputSystem::WindDown() 
{
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().disconnect<&InputSystem::OnMouseEvent>(this);
	Engine::Dispatcher().sink<InputPattern>().disconnect<&InputSystem::Test>(this);
};

Bool InputPattern::IsInputDown(DaggerKey key) const
{
	return keys[(UInt32)key];
}

Bool InputPattern::IsInputDown(DaggerMouse button) const
{
	return mouse[(UInt32)button - 255];
}

UInt32 InputPattern::GetInputLength(DaggerKey key) const
{
	UInt32 value = (UInt32)key;
	if (!moment.contains(value)) return 0;

	return std::chrono::duration_cast<std::chrono::milliseconds>(Engine::CurrentTime() - moment.at(value)).count();
}

UInt32 InputPattern::GetInputLength(DaggerMouse mouse) const
{
	UInt32 value = (UInt32)mouse;
	return std::chrono::duration_cast<std::chrono::milliseconds>(Engine::CurrentTime() - moment.at(value)).count();
}