
#include "engine.h"
#include "render.h"
#include "escape.h"
#include "keylog.h"
#include "frame_count.h"

#include <spdlog/spdlog.h>

int main(int argc_, char** argv_)
{
	spdlog::set_level(spdlog::level::trace);

	dagger::Engine engine;

	engine.AddSystem<dagger::RenderSystem>(1200, 800);
	engine.AddSystem<EscapeSystem>();
	engine.AddSystem<FrameCountSystem>();
	engine.AddSystem<KeyLogSystem>();

	dagger::EngineInit(engine);

	while (engine.Up())
	{
		dagger::EngineLoop(engine);
	}

	dagger::EngineStop(engine);

	return 0;
}
