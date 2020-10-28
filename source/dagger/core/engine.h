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
			srand(time(0));
			Logger::set_level(Logger::level::trace);

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

		void EngineError(Error& error_)
		{
			Logger::error(error_.message);
			exit(-1);
		}

		void EngineInit()
		{
			this->m_EventDispatcher.reset(new entt::dispatcher{});
			this->m_Registry.reset(new entt::registry{});

			this->Dispatcher().sink<Error>().connect<&Engine::EngineError>(*this);

			for (auto& system : this->m_Systems)
			{
				system->SpinUp();
			}
			this->Dispatcher().sink<Exit>().connect<&Engine::EngineShutdown>(*this);
		}


		void EngineLoop()
		{
			Duration frameDuration{};
			static TimePoint lastTime{ TimeSnapshot() };
			static TimePoint nextTime{ TimeSnapshot() };

#if defined(MEASURE_SYSTEMS)
			static TimePoint systemStart{};
			static TimePoint systemEnd{};
#endif//defined(MEASURE_SYSTEMS)

			for (auto& system : this->m_Systems)
			{
#if defined(MEASURE_SYSTEMS)
				systemStart = TimeSnapshot();
#endif//defined(MEASURE_SYSTEMS)
				system->Run();
#if defined(MEASURE_SYSTEMS)
				systemEnd = TimeSnapshot();
				frameDuration += (systemEnd - systemStart);
				Engine::Dispatcher().trigger<SystemRunStats>(SystemRunStats{ system->SystemName(), systemEnd - systemStart });
#endif//defined(MEASURE_SYSTEMS)
			}

			nextTime = TimeSnapshot();
			this->m_DeltaTime = (nextTime - lastTime);
#if !defined(MEASURE_SYSTEMS)
			frameDuration = this->m_DeltaTime;
#endif//!defined(MEASURE_SYSTEMS)
			lastTime = nextTime;
			this->m_CurrentTime = lastTime;
			this->m_FrameCounter++;

			this->Dispatcher().trigger<NextFrame>();
		}

		void EngineStop()
		{
			for (auto& system : this->m_Systems)
			{
				system->WindDown();
			}

			this->m_Systems.clear();

			this->Dispatcher().sink<Error>().disconnect<&Engine::EngineError>(*this);
			this->Dispatcher().sink<Error>().connect<&Engine::EngineError>(*this);

			this->m_EventDispatcher.reset();
			this->m_Registry.reset();
		}
	};
}