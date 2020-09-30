#pragma once

#include "events.h"
#include "system.h"

#include <entt/entt.hpp>
#include <SDL.h>
#include <spdlog/spdlog.h>

#undef main

#include <memory>

namespace dagger
{
	namespace engine
	{
		class Engine
		{
			friend void EngineInit(Engine& engine_);
			friend void EngineStop(Engine& engine_);
			friend void EngineLoop(Engine& engine_);

			std::vector<std::unique_ptr<System>> m_Systems;
			entt::registry m_Registry;
			entt::dispatcher m_EventDispatcher;
			bool m_ShouldStayUp;

		public:
			Engine()
				: m_Systems{}
				, m_Registry{}
				, m_ShouldStayUp{ true }
			{}
			 
			Engine(const Engine&) = delete;

			~Engine() {}

			inline bool Up() const
			{
				return m_ShouldStayUp;
			}

			template<typename Sys>
			inline void AddSystem()
			{
				m_Systems.push_back(std::unique_ptr<Sys>(new Sys()));
			}

			void EngineShutdown(Exit& exit_)
			{
				m_ShouldStayUp = false;
			}
		};

		void EngineError(Error& error_)
		{
			spdlog::error(error_.m_Message);
			exit(-1);
		}

		void EngineInit(Engine& engine_)
		{
			engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
			for (auto& system : engine_.m_Systems)
			{
				system->SpinUp(engine_.m_EventDispatcher);
			}
			engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();

			engine_.m_EventDispatcher.sink<Exit>().connect<&Engine::EngineShutdown>(engine_);
		}


		void EngineStop(Engine& engine_)
		{
			engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
			for (auto& system : engine_.m_Systems)
			{
				system->WindDown(engine_.m_EventDispatcher);
			}
			engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();
		}


		void EngineLoop(Engine& engine_)
		{
			static Frame oneFrame;
			static SDL_Event event;
			while (SDL_PollEvent(&event) != 0)
			{
				switch (event.type)
				{
				case SDL_QUIT:
					engine_.m_ShouldStayUp = false;
					break;

				case SDL_KEYDOWN:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::KEY_DOWN, (Keyboard)event.key.keysym.sym));
					break;

				case SDL_KEYUP:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::KEY_UP, (Keyboard)event.key.keysym.sym));
					break;

				case SDL_MOUSEBUTTONDOWN:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::MOUSE_DOWN, (Mouse)event.button.button));
					break;

				case SDL_MOUSEBUTTONUP:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::MOUSE_UP, (Mouse)event.button.button));
					break;

				case SDL_MOUSEMOTION:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::MOUSE_MOTION, event.motion.x, event.motion.y));
					break;

				case SDL_CONTROLLERBUTTONDOWN:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::GAMEPAD_DOWN, (Gamepad)event.cbutton.button));
					break;

				case SDL_CONTROLLERBUTTONUP:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::GAMEPAD_UP, (Gamepad)event.cbutton.button));
					break;

				case SDL_CONTROLLERAXISMOTION:
					engine_.m_EventDispatcher.trigger<InputEvent>(InputEvent(EventType::GAMEPAD_AXIS, (Gamepad)event.caxis.axis));
					break;
				}
			}

			for (auto& system : engine_.m_Systems)
			{
				system->Run(engine_.m_EventDispatcher);
			}

			engine_.m_EventDispatcher.trigger<Frame>(oneFrame);
		}
	}
}