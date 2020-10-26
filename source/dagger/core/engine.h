#pragma once

#include "system.h"
#include "core/core.h"

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
		, public Publisher<NextFrame>
	{
		friend void EngineInit(Engine& engine_);
		friend void EngineStop(Engine& engine_);
		friend void EngineLoop(Engine& engine_);

		UInt64 m_LastFrameCounter{ 0 };
		UInt64 m_FrameCounter{ 0 };
		Duration m_DeltaTime{ 0.0 };
		TimePoint m_CurrentTime{};

		std::vector<System*> m_Systems;
		OwningPtr<entt::registry> m_Registry;
		OwningPtr<entt::dispatcher> m_EventDispatcher;
		Bool m_ShouldStayUp;
		
		static inline Engine* s_Instance = nullptr;
	public:

		static inline uint64_t s_EntityId = 0;

		static inline Engine& Instance()
		{
			return *s_Instance;
		}
		
		static Float32 DeltaTime()
		{
			return s_Instance->m_DeltaTime.count();
		}

		static inline TimePoint CurrentTime()
		{
			return s_Instance->m_CurrentTime;
		}

		static inline UInt64 FrameCount()
		{
			return s_Instance->m_FrameCounter;
		}

		static inline entt::dispatcher& Dispatcher()
		{
			return s_Instance->GetDispatcher();
		}

		static inline entt::registry& Registry()
		{
			return s_Instance->GetRegistry();
		}

		template<typename K, typename T>
		inline static tsl::sparse_map<K, T>& Cache()
		{
			static tsl::sparse_map<K, T> s_CachedMap;
			return s_CachedMap;
		}

		template<typename T>
		inline static tsl::sparse_map<std::string, T*>& Res()
		{
			static tsl::sparse_map<std::string, T*> s_CachedMap;
			return s_CachedMap;
		}

		Engine()
			: m_Systems{}
			, m_Registry{}
			, m_EventDispatcher{}
			, m_ShouldStayUp{ true }
		{
			Engine::s_Instance = this;
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
			m_Systems.push_back(new Sys(std::forward<Args>(args_)...));
		}

		void EngineShutdown(Exit&)
		{
			m_ShouldStayUp = false;
		}

		inline entt::dispatcher& GetDispatcher()
		{
			return *m_EventDispatcher.get();
		}

		inline entt::registry& GetRegistry()
		{
			return *m_Registry.get();
		}
	};

	static void EngineError(Error& error_)
	{
		Logger::error(error_.message);
		exit(-1);
	}

	static void EngineInit(Engine& engine_)
	{
		engine_.m_EventDispatcher.reset(new entt::dispatcher{});
		engine_.m_Registry.reset(new entt::registry{});

		engine_.Dispatcher().sink<Error>().connect<&EngineError>();

		for (auto& system : engine_.m_Systems)
		{
			system->SpinUp();
		}
		engine_.Dispatcher().sink<Exit>().connect<&Engine::EngineShutdown>(engine_);
	}


	static void EngineLoop(Engine& engine_)
	{
		Duration frameDuration{};
		static TimePoint lastTime{ std::chrono::steady_clock::now() };
		static TimePoint nextTime{ std::chrono::steady_clock::now() };
		
#if defined(MEASURE_SYSTEMS)
		static TimePoint systemStart{};
		static TimePoint systemEnd{};
#endif//defined(MEASURE_SYSTEMS)

		for (auto& system : engine_.m_Systems)
		{
#if defined(MEASURE_SYSTEMS)
			systemStart = std::chrono::steady_clock::now();
#endif//defined(MEASURE_SYSTEMS)
			system->Run();
#if defined(MEASURE_SYSTEMS)
			systemEnd = std::chrono::steady_clock::now();
			frameDuration += (systemEnd - systemStart);
			Engine::Dispatcher().trigger<SystemRunStats>(SystemRunStats{ system->SystemName(), systemEnd - systemStart });
#endif//defined(MEASURE_SYSTEMS)
		}

		nextTime = std::chrono::steady_clock::now();
		engine_.m_DeltaTime = (nextTime - lastTime);
#if !defined(MEASURE_SYSTEMS)
		frameDuration = engine_.m_DeltaTime;
#endif//!defined(MEASURE_SYSTEMS)
		lastTime = nextTime;
		engine_.m_CurrentTime = lastTime;
		engine_.m_FrameCounter++;

		engine_.Dispatcher().trigger<NextFrame>();
	}


	static void EngineStop(Engine& engine_)
	{
		for (auto& system : engine_.m_Systems)
		{
			system->WindDown();
		}

		engine_.m_Systems.clear();

		engine_.Dispatcher().sink<Error>().disconnect<&EngineError>();
		engine_.Dispatcher().sink<Error>().connect<&EngineError>();

		engine_.m_EventDispatcher.reset();
		engine_.m_Registry.reset();
	}
}