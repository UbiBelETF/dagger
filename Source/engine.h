#pragma once

#include "system.h"
#include "structures.h"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <tsl/sparse_map.h>
#include <tsl/sparse_set.h>

#include <utility>
#include <memory>
#include <typeinfo>

#undef main

namespace dagger
{
	class Engine 
		: public Subscriber<Exit, Error>
		, public Publisher<Frame>
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

		static inline uint64_t ms_EntityId = 0;

		static inline Engine& Instance()
		{
			return *ms_Instance;
		}

		static inline entt::dispatcher& Dispatch()
		{
			return ms_Instance->GetDispatcher();
		}

		static inline entt::registry& Registry()
		{
			return ms_Instance->GetRegistry();
		}

		template<typename K, typename T>
		inline static tsl::sparse_map<K, T>& Cache()
		{
#if !defined(NDEBUG)
			spdlog::trace("<{}, {}> added to engine cache.", typeid(K).name(), typeid(T).name());
#endif
			static tsl::sparse_map<K, T> s_CachedMap;
			return s_CachedMap;
		}

		template<typename T>
		inline static T& Cache(std::string name_)
		{
			static tsl::sparse_map<std::string, T> s_CachedMap;
#if !defined(NDEBUG)
			spdlog::trace("<{}>({}) added to engine cache.", typeid(T).name(), name_);
#endif
			return s_CachedMap[name_];
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

		~Engine() 
		{}

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

	static void EngineError(Error& error_)
	{
		spdlog::error(error_.m_Message);
		exit(-1);
	}

	static void EngineInit(Engine& engine_)
	{
		engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
		for (auto& system : engine_.m_Systems)
		{
			system->SpinUp(engine_);
		}
		engine_.m_EventDispatcher.sink<Exit>().connect<&Engine::EngineShutdown>(engine_);
	}


	static void EngineLoop(Engine& engine_)
	{
		for (auto& system : engine_.m_Systems)
		{
			system->Run(engine_);
		}

		engine_.m_EventDispatcher.trigger<Frame>();
	}


	static void EngineStop(Engine& engine_)
	{
		for (auto system = engine_.m_Systems.rbegin(); system != engine_.m_Systems.rend(); system++)
		{
			(*system)->WindDown(engine_);
			(*system).reset();
		}
		engine_.m_EventDispatcher.sink<Error>().disconnect<&EngineError>();
		engine_.m_EventDispatcher.sink<Error>().connect<&EngineError>();
	}
}