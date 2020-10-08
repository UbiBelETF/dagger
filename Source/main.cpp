
#include "engine.h"
#include "window.h"
#include "diag.h"
#include "gui.h"
#include "console.h"
#include "test.h"
#include "toolmenu.h"

#include <spdlog/spdlog.h>

using namespace dagger;

int main(int argc_, char** argv_)
{
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>(1200, 800);
	engine.AddSystem<GUISystem>();
	engine.AddSystem<DiagnosticSystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<TestSystem>();
	engine.AddSystem<ToolMenuSystem>();
	
	EngineInit(engine);

	auto &reg = engine.GetRegistry();
	for (int i = 0; i < 10; i++)
	{
		auto entity = reg.create();
		reg.emplace<Sprite>(entity, ++Engine::ms_EntityId, ((rand() % 10) - 5) * 0.01f, ((rand() % 10) - 5) * 0.01f);
	}

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
