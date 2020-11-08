#include "core/engine.h"
#include "core/game.h"

#include <SimpleIni.h>

using namespace dagger;

Engine::Engine()
	: m_Game{}
	, m_Ini{}
	, m_Systems{}
	, m_Registry{}
	, m_EventDispatcher{}
	, m_ShouldStayUp{ true }
	, m_ExitStatus{ 0 }
{
	srand(time(0));
	Logger::set_level(Logger::level::trace);

	Engine::s_Instance = this;
}

void Engine::EngineShutdown(Exit&)
{
	Logger::error("Engine shutdown called.");
	m_ShouldStayUp = false;
}

void Engine::EngineError(Error& error_)
{
	Logger::error(error_.message);
	m_ShouldStayUp = false;
	m_ExitStatus = -1;
}

void Engine::EngineInit()
{
	this->m_EventDispatcher.reset(new entt::dispatcher{});
	this->m_Registry.reset(new entt::registry{});

	this->Dispatcher().sink<Error>().connect<&Engine::EngineError>(*this);

	for (auto& system : this->m_Systems)
	{
		system->SpinUp();
		if (m_ExitStatus != 0)
		{
			Logger::critical("Spinning up failed: {}", system->SystemName());
			break;
		}
	}
	this->Dispatcher().sink<Exit>().connect<&Engine::EngineShutdown>(*this);
}

void Engine::EngineLoop()
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

void Engine::EngineStop()
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
