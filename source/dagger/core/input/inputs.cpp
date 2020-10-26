#include "inputs.h"
#include "core/engine.h"

#include <GLFW/glfw3.h>

using namespace dagger;

void InputSystem::OnKeyboardEvent(KeyboardEvent input_)
{
	auto key = input_.key;

	if (input_.action == GLFW_PRESS)
		m_Mapping.Keys[key] = true;
	else if (input_.action == GLFW_RELEASE)
		m_Mapping.Keys[key] = false;
}

void InputSystem::OnMouseEvent(MouseEvent input_)
{
	auto key = input_.button;

	if (input_.action == GLFW_PRESS)
		m_Mapping.Mouse[key] = true;
	else if (input_.action == GLFW_RELEASE)
		m_Mapping.Mouse[key] = false;
}

void InputSystem::SpinUp() 
{
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().connect<&InputSystem::OnMouseEvent>(this);
};

void InputSystem::Run()
{
	for (auto& context : m_InputContexts)
	{
		context.ConsumeInput(m_Mapping);
	}
};

void InputSystem::WindDown() 
{
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().disconnect<&InputSystem::OnMouseEvent>(this);
};
