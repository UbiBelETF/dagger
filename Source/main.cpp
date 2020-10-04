
#include "engine.h"
#include "window.h"
#include "diag.h"
#include "gui.h"
#include "console.h"
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
	engine.AddSystem<ToolMenuSystem>();

	EngineInit(engine);

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
