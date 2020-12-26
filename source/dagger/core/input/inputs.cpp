
#include "inputs.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/window.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace dagger;

void InputSystem::OnKeyboardEvent(KeyboardEvent input_)
{
    if ((SInt32)input_.key < 0) 
    {
	    return;
    }
	
	auto key = (UInt64)input_.key;


	if (input_.action == EDaggerInputState::Pressed)
	{
		m_InputState.keys[key] = true;
		m_InputState.moments[key] = Engine::CurrentTime();
		m_InputState.bitmap.set(key);
	}
	else if (input_.action == EDaggerInputState::Released)
	{
		m_InputState.releasedLastFrame.emplace(key);
		m_InputState.keys[key] = false;
		m_InputState.moments.erase(key);
		// NOTE: not a bug! bitmap not reset here but after next update via `justReleased`!
	}
}

void InputSystem::OnMouseEvent(MouseEvent input_)
{
	UInt32 button = (UInt64)input_.button;

	if (input_.action == EDaggerInputState::Pressed)
	{
		m_InputState.mouse[button - MouseStart] = true;
		m_InputState.moments[button] = std::chrono::steady_clock::now();
		m_InputState.bitmap.set(button);
	}
	else if (input_.action == EDaggerInputState::Released)
	{
		m_InputState.releasedLastFrame.emplace(button);
		m_InputState.mouse[button - MouseStart] = false;
		m_InputState.moments.erase(button);
		// NOTE: not a bug! bitmap not reset here but after next update via `justReleased`!
	}
}

void InputSystem::OnCursorMoveEvent(CursorEvent cursor_)
{
	m_InputState.cursor = cursor_;
}

void InputSystem::SpinUp()
{
	Engine::Dispatcher().sink<AssetLoadRequest<InputContext>>().connect<&InputSystem::OnAssetLoadRequest>(this);
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().connect<&InputSystem::OnMouseEvent>(this);
	Engine::Dispatcher().sink<CursorEvent>().connect<&InputSystem::OnCursorMoveEvent>(this);

	Engine::PutDefaultResource<InputState>(&m_InputState);

	LoadDefaultAssets();
};

void InputSystem::LoadDefaultAssets()
{
	for (auto& entry : Files::recursive_directory_iterator("input-contexts"))
	{
		auto path = entry.path().string();
		if (entry.is_regular_file() && entry.path().extension() == ".json")
		{
			Engine::Dispatcher().trigger<AssetLoadRequest<InputContext>>(AssetLoadRequest<InputContext>{ path });
		}
	}
}

void InputSystem::LoadInputAction(InputCommand& command_, JSON::json& input_)
{
	InputAction action;
	assert(input_.contains("trigger"));
	assert(s_InputValues.contains(input_["trigger"]));
	action.trigger = s_InputValues[input_["trigger"]];

	if (input_.contains("duration"))
	{
		action.duration = input_["duration"];
	}
	else
		action.duration = 0;

	if (input_.contains("event"))
	{
		String eventHandler = input_["event"];
		if (eventHandler == "Pressed")
			action.event = EDaggerInputState::Pressed;
		else if (eventHandler == "Held")
			action.event = EDaggerInputState::Held;
		else if (eventHandler == "Released")
			action.event = EDaggerInputState::Released;
	}
	else
		action.event = EDaggerInputState::Held;

	if (input_.contains("value"))
	{
		action.value = input_["value"];
	}
	else
		action.value = 1;

	command_.actions.push_back(std::move(action));
}

void InputSystem::OnAssetLoadRequest(AssetLoadRequest<InputContext> request_)
{
	FilePath path(request_.path);
	Logger::info("Loading '{}'", request_.path);

	if (!Files::exists(path))
	{
		Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Couldn't load input context from {}.", request_.path) });
		return;
	}

	FileInputStream handle;
	auto absolutePath = Files::absolute(path);
	handle.open(absolutePath);

	if (!handle.is_open())
	{
		Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Couldn't open input context file '{}' for reading.", absolutePath.string()) });
		return;
	}

	JSON::json json;
	handle >> json;

	InputContext* context = new InputContext();
	assert(json.contains("context-name"));
	assert(json.contains("commands"));

	context->name = json["context-name"];
	for (auto& cmd : json["commands"])
	{
		InputCommand command;
		assert(cmd.contains("command-name"));
		command.name = cmd["command-name"];

		if (cmd.contains("actions"))
		{
			for (auto& action : cmd["actions"])
			{
				LoadInputAction(command, action);
			}
		}
		else
		{
			LoadInputAction(command, cmd);
		}

		for (auto& action : command.actions)
		{
			context->bitmap.set(action.trigger, true);
		}

		context->commands.push_back(std::move(command));
	}

	auto& library = Engine::Res<InputContext>();
	if (library.contains(context->name))
	{
		delete library[context->name];
	}

	library[context->name] = context;
	Logger::info("Input context '{}' loaded!", context->name);
}

void InputSystem::Run()
{
	Engine::Registry().view<InputReceiver>().each([&](InputReceiver& receiver_)
		{
			static Set<String> updatedCommands{};

			auto& bitmap = m_InputState.bitmap;
			auto& library = Engine::Res<InputContext>();
			for (auto& name : receiver_.contexts)
			{
				assert(library.contains(name));
				const auto& context = library[name];
				const auto& collision = (bitmap & context->bitmap);
				if (collision.any())
				{
					for (auto& command : context->commands)
					{
						String fullName = command.name;
						for (auto& action : command.actions)
						{
							if (action.event == EDaggerInputState::Released)
							{
								if (m_InputState.releasedLastFrame.contains(action.trigger))
								{
									receiver_.values[fullName] = action.value;
									updatedCommands.emplace(fullName);
								}
							}
							else
							{
								if (m_InputState.releasedLastFrame.contains(action.trigger))
								{
									receiver_.values[fullName] = 0;
								}

								Bool toFire = false;
								const Bool toConsume = action.event == EDaggerInputState::Pressed;

								if (action.duration == 0)
								{
									// mouse
									if (action.trigger >= MouseStart && action.trigger <= (MouseStart + 10))
									{
										if (Input::IsInputDown((EDaggerMouse)(action.trigger)))
										{
											toFire = true;
											if (toConsume)
												m_InputState.releasedLastFrame.emplace(action.trigger);
										}
									}
									else
									{
										if (Input::IsInputDown((EDaggerKeyboard)(action.trigger)))
										{
											toFire = true;
											if (toConsume) m_InputState.releasedLastFrame.emplace(action.trigger);
										}
									}
								}
								else
								{
									// mouse
									if (action.trigger >= MouseStart && action.trigger <= (MouseStart + 10))
									{
										if (Input::GetInputDuration((EDaggerMouse)(action.trigger)) >= action.duration)
										{
											toFire = true;
											m_InputState.releasedLastFrame.emplace(action.trigger);
										}
									}
									else
									{
										if (Input::GetInputDuration((EDaggerKeyboard)(action.trigger)) >= action.duration)
										{
											toFire = true;
											m_InputState.releasedLastFrame.emplace(action.trigger);
										}
									}
								}

								if (toFire)
								{
									receiver_.values[fullName] = action.value;
									updatedCommands.emplace(fullName);
								}
							}
						}
					}
				}

				Map<String, Float32> newValues;
				for (auto [key, value] : receiver_.values)
				{
					newValues[key] = updatedCommands.contains(key) ? value : 0.0f;
				}

				receiver_.values = newValues;
				updatedCommands.clear();
			}
		});

	if (!m_InputState.releasedLastFrame.empty())
	{
		for (auto& input : m_InputState.releasedLastFrame)
		{
			m_InputState.bitmap.reset(input);
		}

		m_InputState.releasedLastFrame.clear();
	}
};

void InputSystem::WindDown()
{
	Engine::Dispatcher().sink<AssetLoadRequest<InputContext>>().disconnect<&InputSystem::OnAssetLoadRequest>(this);
	Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&InputSystem::OnKeyboardEvent>(this);
	Engine::Dispatcher().sink<MouseEvent>().disconnect<&InputSystem::OnMouseEvent>(this);
	Engine::Dispatcher().sink<CursorEvent>().disconnect<&InputSystem::OnCursorMoveEvent>(this);
};

Bool dagger::Input::IsInputDown(EDaggerKeyboard key_)
{
	const auto* state = Engine::GetDefaultResource<InputState>();
	return state->keys[(UInt32)key_];
}

Bool dagger::Input::IsInputDown(EDaggerMouse button_)
{
	const auto* state = Engine::GetDefaultResource<InputState>();
	return state->mouse[(UInt32)button_ - MouseStart];
}

const Vector2 dagger::Input::CursorPositionInWindow()
{
	const auto* state = Engine::GetDefaultResource<InputState>();
	return state->cursor;
}

const Vector2 dagger::Input::CursorPositionInScreen()
{
	auto* state = Engine::GetDefaultResource<InputState>();
	return Camera::WindowToScreen(state->cursor);
}

const Vector2 dagger::Input::CursorPositionInWorld()
{
	auto* state = Engine::GetDefaultResource<InputState>();
	return Camera::WindowToWorld(state->cursor);
}

UInt32 dagger::Input::GetInputDuration(EDaggerKeyboard key_)
{
	const auto* state = Engine::GetDefaultResource<InputState>();
	UInt32 value = (UInt32)key_;
	if (!state->moments.contains(value)) 
	{ 
		return 0;
	}

	return DurationToMilliseconds(Engine::CurrentTime() - state->moments.at(value));
}

UInt32 dagger::Input::GetInputDuration(EDaggerMouse mouse_)
{
	const auto* state = Engine::GetDefaultResource<InputState>();
	UInt32 value = (UInt32)mouse_;
	return DurationToMilliseconds(Engine::CurrentTime() - state->moments.at(value));
}

void dagger::Input::ConsumeInput(EDaggerKeyboard key_)
{
	auto* state = Engine::GetDefaultResource<InputState>();

	UInt32 value = (UInt32)key_;
	state->keys[value] = false;
	state->moments.erase(value);
	state->bitmap.reset(value);
}

void dagger::Input::ConsumeInput(EDaggerMouse button_)
{
	auto* state = Engine::GetDefaultResource<InputState>();

	UInt32 value = (UInt32)button_;
	state->mouse[value - MouseStart] = false;
	state->moments.erase(value);
	state->bitmap.reset(value);
}
