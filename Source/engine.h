#pragma once

#include "structures.h"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>

#include <utility>
#include <memory>

#undef main

namespace dagger
{
	class Engine;

	struct System
	{
		virtual void SpinUp(Engine&) {};
		virtual void Run(Engine&) {};
		virtual void WindDown(Engine&) {};
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

		static inline Engine* ms_Instance = nullptr;
	public:

		static inline Engine& Instance()
		{
			return *ms_Instance;
		}

		static inline entt::dispatcher& Dispatch()
		{
			return ms_Instance->GetDispatcher();
		}

		template<typename K, typename T>
		inline static tsl::sparse_map<K, T>& Cache()
		{
			static tsl::sparse_map<K, T> s_CachedMap;
			return s_CachedMap;
		}

		template<typename T>
		inline static T& Cache()
		{
			static T s_CachedElement{};
			return s_CachedElement;
		}

		
		Engine()
			: m_Systems{}
			, m_Registry{}
			, m_EventDispatcher{}
			, m_ShouldStayUp{ true }
		{
			Engine::ms_Instance = this;
		}
			 
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
			system->SpinUp(engine_);
		}
		engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();
		engine_.m_EventDispatcher.sink<Exit>().connect<&Engine::EngineShutdown>(engine_);
	}


	void EngineLoop(Engine& engine_)
	{
		static Frame oneFrame;

		for (auto& system : engine_.m_Systems)
		{
			system->Run(engine_);
		}

		engine_.m_EventDispatcher.trigger<Frame>(oneFrame);
	}


	void EngineStop(Engine& engine_)
	{
		engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
		for (auto system = engine_.m_Systems.rbegin(); system != engine_.m_Systems.rend(); system++)
		{
			(*system)->WindDown(engine_);
		}
		engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();
	}
}