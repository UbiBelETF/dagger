#pragma once

#include "events.h"
#include "texture.h"
#include "engine.h"

#include <entt/entt.hpp>
#include <SDL.h>
#include <spdlog/spdlog.h>
#include <tsl/sparse_map.h>

#include <utility>
#include <memory>

#undef main

namespace dagger
{
	struct Frame
	{};

	struct Exit
	{};

	struct Error
	{
		std::string m_Message;

		Error(std::string message_)
			: m_Message{ message_ }
		{}

		Error() = delete;

		Error(const Error&) = default;
		Error& operator=(const Error&) = default;
		Error(Error&&) = default;
		Error& operator=(Error&&) = default;

		~Error() = default;
	};

	class Engine;

	struct System
	{
		virtual void SpinUp(Engine&) = 0;
		virtual void Run(Engine&) = 0;
		virtual void WindDown(Engine&) = 0;
	};

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
		/* type-specific storage */
		static tsl::sparse_map<uint32_t, SDL_Texture*> ms_Textures;
		/* type-specific storage */

		Engine()
			: m_Systems{}
			, m_Registry{}
			, m_EventDispatcher{}
			, m_ShouldStayUp{ true }
		{}
			 
		Engine(const Engine&) = delete;

		~Engine() {}

		inline bool Up() const
		{
			return m_ShouldStayUp;
		}

		template<typename Sys, typename... Args>
		inline void AddSystem(Args&&... args_)
		{
			m_Systems.push_back(std::unique_ptr<Sys>(new Sys(std::forward<Args>(args_)...)));
		}

		void EngineShutdown(Exit& exit_)
		{
			m_ShouldStayUp = false;
		}

		inline entt::dispatcher& GetDispatcher()
		{
			return m_EventDispatcher;
		}

		inline entt::registry& GetRegistry()
		{
			return m_Registry;
		}
	};

	tsl::sparse_map<uint32_t, SDL_Texture*> Engine::ms_Textures{};

	void EngineError(Error& error_)
	{
		spdlog::error(error_.m_Message);
		exit(-1);
	}

	void EngineCacheTexture(TextureLoaded texture_)
	{
		spdlog::info("Loaded and cached texture {} (id #{})", texture_.m_Path, texture_.m_Id);
		Engine::ms_Textures[texture_.m_Id] = texture_.m_Texture;
	}

	void EngineInit(Engine& engine_)
	{
		engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
		for (auto& system : engine_.m_Systems)
		{
			system->SpinUp(engine_);
		}
		engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();

		engine_.m_EventDispatcher.sink<TextureLoaded>().connect<&EngineCacheTexture>();
		engine_.m_EventDispatcher.sink<Exit>().connect<&Engine::EngineShutdown>(engine_);
	}


	void EngineStop(Engine& engine_)
	{
		engine_.m_EventDispatcher.sink<TextureLoaded>().disconnect<&EngineCacheTexture>();
		engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
		for (auto& system : engine_.m_Systems)
		{
			system->WindDown(engine_);
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
			system->Run(engine_);
		}

		engine_.m_EventDispatcher.trigger<Frame>(oneFrame);
	}
}