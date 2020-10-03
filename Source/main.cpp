
#include "engine.h"
#include "render.h"
#include "fps.h"
#include "gui.h"

#include <spdlog/spdlog.h>

using namespace dagger;

int main(int argc_, char** argv_)
{
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<RenderSystem>(1200, 800);
	engine.AddSystem<GUISystem>();
	engine.AddSystem<FPSSystem>();

	EngineInit(engine);

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
